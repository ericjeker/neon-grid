// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FireProjectileAbility.generated.h"

/**
 * Ability that spawns a projectile in front of the character.
 */
UCLASS()
class NEONGRID_API UFireProjectileAbility : public UGameplayAbility
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
	/** 
	 * Class of projectile to spawn 
	 * @deprecated Use WeaponData in the Pawn to know which ProjectileClass to spawn
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Projectile")
	TSubclassOf<class ABaseProjectile> ProjectileClass;
};
