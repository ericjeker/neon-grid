// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "NeonGameplayAbility_FireEquipment.h"

#include "NeonEquipment.h"
#include "NeonLoadoutComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UNeonGameplayAbility_FireEquipment::UNeonGameplayAbility_FireEquipment()
{
	// Default settings (override from parent)
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UNeonGameplayAbility_FireEquipment::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                         const FGameplayAbilityActorInfo* ActorInfo,
                                                         const FGameplayAbilityActivationInfo ActivationInfo,
                                                         const FGameplayEventData* TriggerEventData)
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
		// 3. Use the Current Equipment
		if (ANeonEquipment* Target = LoadoutComponent->GetCurrentEquipment())
		{
			UE_LOG(LogTemp, Warning, TEXT("Fire equipment %s"), *Target->GetName());
			
			// Perform the interaction (Validate & Execute)
			Target->StartPrimaryAction();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No equipment equipped"));
		}
	}

	// 4. End the ability immediately (Fire-and-Forget)
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
