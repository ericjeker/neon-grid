// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponData.generated.h"

/**
 * @class UWeaponData
 * @brief Represents weapon configuration data for combat systems.
 *
 * This class is used to store and manage configuration data for weapons,
 * including range parameters, fire rate, and associated projectile and abilities.
 * It inherits from UDataAsset, enabling easy reuse and modification of weapon
 * data assets through Unreal's content system.
 */
UCLASS()
class NEONGRID_API UWeaponData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float OptimalRange = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float MinEffectiveRange = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float MaxEffectiveRange = 1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float FireRate = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TSubclassOf<class ABaseProjectile> ProjectileClass;
};
