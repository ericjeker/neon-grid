// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "NeonGameplayAbility_CycleEquipment.generated.h"

/**
 * Ability to switch between equipment in the loadout.
 */
UCLASS()
class NEONGRID_API UNeonGameplayAbility_CycleEquipment : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UNeonGameplayAbility_CycleEquipment();

	/** The main entry point when the ability is triggered */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;
};
