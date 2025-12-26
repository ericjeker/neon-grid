// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "BTTask_ApproachTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "NeonGrid/Core/NeonGridEnums.h"


UBTTask_ApproachTarget::UBTTask_ApproachTarget()
{
	NodeName = TEXT("Approach Target");
	bNotifyTick = true;

	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_ApproachTarget, TargetActorKey),
	                               AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_ApproachTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	const APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}

	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));

	if (!TargetActor)
	{
		return EBTNodeResult::Failed;
	}

	// Get optimal range from blackboard (set by WeaponEquipmentComponent)
	float DesiredRange = FallbackRange;
	if (bUseWeaponOptimalRange)
	{
		const float WeaponOptimalRange = BlackboardComp->GetValueAsFloat(NeonGridAIKeys::WeaponOptimalRange);
		if (WeaponOptimalRange > 0.f)
		{
			DesiredRange = WeaponOptimalRange;
		}
	}

	// Check if already in range
	const float CurrentDistance = FVector::Dist(ControlledPawn->GetActorLocation(), TargetActor->GetActorLocation());
	if (FMath::Abs(CurrentDistance - DesiredRange) <= RangeTolerance)
	{
		return EBTNodeResult::Succeeded; // Already in optimal range
	}

	// Move to the optimal range
	EPathFollowingRequestResult::Type MoveResult = AIController->MoveToActor(
		TargetActor,
		DesiredRange - RangeTolerance, // Stop a bit before optimal
		true, // Stop on overlap
		true, // Use pathfinding
		true, // Can strafe
		nullptr // Default nav filter
	);

	return MoveResult == EPathFollowingRequestResult::RequestSuccessful
		       ? EBTNodeResult::InProgress
		       : EBTNodeResult::Failed;
}

void UBTTask_ApproachTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	const APawn* ControlledPawn = AIController ? AIController->GetPawn() : nullptr;
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	const AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));

	if (!ControlledPawn || !TargetActor)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	const UPathFollowingComponent* PathComp = AIController->GetPathFollowingComponent();
    
	// Check if the movement system has given up
	if (PathComp->GetStatus() == EPathFollowingStatus::Idle)
	{
		// If we are Idle but NOT at our destination, we are stuck
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// Get current DesiredRange again (in case the weapon changed mid-walk)
	float DesiredRange = FallbackRange;
	const float WeaponOptimalRange = BlackboardComp->GetValueAsFloat(NeonGridAIKeys::WeaponOptimalRange);
	if (bUseWeaponOptimalRange && WeaponOptimalRange > 0.f) DesiredRange = WeaponOptimalRange;

	// Check if we have arrived at the optimal range
	const float CurrentDistance = FVector::Dist(ControlledPawn->GetActorLocation(), TargetActor->GetActorLocation());
    
	if (FMath::Abs(CurrentDistance - DesiredRange) <= RangeTolerance)
	{
		// Stop moving and finish successfully!
		AIController->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
