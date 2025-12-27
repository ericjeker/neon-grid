// Copyright Eric Jeker, Inc. All Rights Reserved.

#include "WeaponEquipmentComponent.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NeonGrid/Core/NeonGridEnums.h"


UWeaponEquipmentComponent::UWeaponEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UWeaponEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	// Initialize blackboard with the current weapon
	if (CurrentWeapon)
	{
		UpdateBlackboard();
	}
}

void UWeaponEquipmentComponent::EquipWeapon(UWeaponData* NewWeapon)
{
	if (!NewWeapon)
	{
		return;
	}

	CurrentWeapon = NewWeapon;
	UpdateBlackboard();
	
	// TODO: Notify of weapon change to make it customizable in Blueprints
}

void UWeaponEquipmentComponent::UpdateBlackboard() const
{
	const APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn)
	{
		return;
	}

	AAIController* AIController = Cast<AAIController>(OwnerPawn->GetController());
	if (!AIController)
	{
		return;
	}

	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	if (!Blackboard || !CurrentWeapon)
	{
		return;
	}

	// Update blackboard with weapon stats
	Blackboard->SetValueAsFloat(NeonGridAIKeys::WeaponOptimalRange, CurrentWeapon->OptimalRange);
	Blackboard->SetValueAsFloat(NeonGridAIKeys::WeaponMinRange, CurrentWeapon->MinEffectiveRange);
	Blackboard->SetValueAsFloat(NeonGridAIKeys::WeaponMaxRange, CurrentWeapon->MaxEffectiveRange);
	Blackboard->SetValueAsFloat(NeonGridAIKeys::WeaponFireRate, CurrentWeapon->FireRate);
}
