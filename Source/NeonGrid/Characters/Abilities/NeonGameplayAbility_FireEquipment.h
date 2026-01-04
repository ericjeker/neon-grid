// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "NeonGameplayAbility_FireEquipment.generated.h"


UCLASS()
class NEONGRID_API UNeonGameplayAbility_FireEquipment : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UNeonGameplayAbility_FireEquipment();

	/** The main entry point when the ability is triggered */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;
};
