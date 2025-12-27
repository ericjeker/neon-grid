// Copyright Eric Jeker, Inc. All Rights Reserved.

#include "BTTask_FireProjectile.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "NeonGrid/Core/NeonGridEnums.h"
#include "DrawDebugHelpers.h"

UBTTask_FireProjectile::UBTTask_FireProjectile()
{
	NodeName = "Fire Projectile at Target";
	
	// Accept only Object types for target
	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FireProjectile, TargetActorKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_FireProjectile::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}

	// Get target actor from Blackboard
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	const AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
	
	if (!TargetActor)
	{
		return EBTNodeResult::Failed;
	}

	// Get weapon range from blackboard (dynamically updated by equipment component)
	float ActualMinRange = BlackboardComp->GetValueAsFloat(NeonGridAIKeys::WeaponMinRange);
	float ActualMaxRange = BlackboardComp->GetValueAsFloat(NeonGridAIKeys::WeaponMaxRange);

	// Fallback to defaults if not set
	if (ActualMinRange <= 0.f) ActualMinRange = MinFireDistance;
	if (ActualMaxRange <= 0.f) ActualMaxRange = MaxFireDistance;

	// Check distance
	const float DistanceToTarget = FVector::Dist(ControlledPawn->GetActorLocation(), TargetActor->GetActorLocation());
	if (DistanceToTarget < ActualMinRange || DistanceToTarget > ActualMaxRange)
	{
		return EBTNodeResult::Failed;
	}

	// Check line of sight
	if (bRequireLineOfSight)
	{
		FHitResult HitResult;
		const FVector StartLocation = ControlledPawn->GetActorLocation();
		const FVector EndLocation = TargetActor->GetActorLocation();

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(ControlledPawn);

		const bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			ECC_Visibility,
			QueryParams
		);

		// If we hit something and it's not the target, we don't have LOS
		if (bHit && HitResult.GetActor() != TargetActor)
		{
			return EBTNodeResult::Failed;
		}
	}

	// Try to activate the fire ability via GAS
	IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(ControlledPawn);
	if (AbilitySystemInterface)
	{
		UAbilitySystemComponent* ASC = AbilitySystemInterface->GetAbilitySystemComponent();
		if (ASC)
		{
			// Try to activate ability by Input ID (assuming Fire is mapped)
			// You can also use ability tags if you prefer
			FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(static_cast<int32>(EAbilityInputID::Fire));
			if (Spec)
			{
				ASC->TryActivateAbility(Spec->Handle);
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}