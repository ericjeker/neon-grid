// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FireProjectileAbility.generated.h"

/**
 * Ability that spawns a projectile in front of the character.
 */
UCLASS()
class /**
 * Activate the ability when it is triggered, performing the fire action for the owning actor.
 *
 * @param Handle Gameplay ability spec handle identifying this ability instance.
 * @param ActorInfo Information about the actor that owns and activates the ability.
 * @param ActivationInfo Contextual activation data for this ability activation.
 * @param TriggerEventData Optional event data that triggered the ability activation.
 */
/**
 * Spawn a projectile for this ability's owner using the configured projectile class and firing context.
 */
NEONGRID_API UFireProjectileAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFireProjectileAbility();

	/** The main entry point when the ability is triggered */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintNativeEvent, Category = "Ability|Projectile")
	void SpawnProjectile();
	
protected:
	/** Class of projectile to spawn */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Projectile")
	TSubclassOf<class ABaseProjectile> ProjectileClass;

	/** TODO: Montage to play when firing */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Projectile")
	UAnimMontage* FireMontage;
};