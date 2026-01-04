// Copyright Eric Jeker, Inc. All Rights Reserved.

#include "NeonGameplayAbility_CycleEquipment.h"

#include "NeonLoadoutComponent.h"


UNeonGameplayAbility_CycleEquipment::UNeonGameplayAbility_CycleEquipment()
{
	// Default settings (override from parent)
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UNeonGameplayAbility_CycleEquipment::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// 1. Get the Avatar (Character/Pawn)
	const AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	if (!AvatarActor)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// 2. Find the Loadout Component on the Avatar
	if (UNeonLoadoutComponent* LoadoutComponent = AvatarActor->FindComponentByClass<UNeonLoadoutComponent>())
	{
		// 3. Use the Component logic
		LoadoutComponent->CycleEquipmentSlot();
	}

	// 4. End the ability immediately (Fire-and-Forget)
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
