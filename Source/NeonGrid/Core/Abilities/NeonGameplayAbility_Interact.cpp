// Copyright Eric Jeker, Inc. All Rights Reserved.

#include "NeonGameplayAbility_Interact.h"
#include "NeonInteractionComponent.h"


UNeonGameplayAbility_Interact::UNeonGameplayAbility_Interact()
{
	// Ability configuration
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UNeonGameplayAbility_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
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

	// 2. Find the Interaction Component on the Avatar
	if (UNeonInteractionComponent* InteractionComp = AvatarActor->FindComponentByClass<UNeonInteractionComponent>())
	{
		// 3. Use the Component logic we wrote earlier
		if (AActor* Target = InteractionComp->GetInteractableUnderCursor(true))
		{
			UE_LOG(LogTemp, Warning, TEXT("Interacting with %s"), *Target->GetName());
			
			// Perform the interaction (Validate & Execute)
			InteractionComp->Interact(Target);
		}
	}

	// 4. End the ability immediately (Fire-and-Forget)
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}