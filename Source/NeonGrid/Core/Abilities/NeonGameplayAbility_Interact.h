// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "NeonGameplayAbility_Interact.generated.h"

/**
 * Ability that triggers the interaction logic on the Avatar's NeonInteractionComponent.
 */
UCLASS()
class NEONGRID_API UNeonGameplayAbility_Interact : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UNeonGameplayAbility_Interact();

	/** Actually activate the ability */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};