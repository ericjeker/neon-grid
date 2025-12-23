// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FireProjectile.generated.h"

/**
 * Ability that spawns a projectile in front of the character.
 */
UCLASS()
class NEONGRID_API UGA_FireProjectile : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_FireProjectile();

	/** The main entry point when the ability is triggered */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	/** Class of projectile to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TSubclassOf<class ABaseProjectile> ProjectileClass;

	/** TODO: Montage to play when firing */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	UAnimMontage* FireMontage;
};
