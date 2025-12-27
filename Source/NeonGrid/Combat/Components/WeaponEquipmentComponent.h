// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NeonGrid/Combat/WeaponData.h"

#include "WeaponEquipmentComponent.generated.h"

UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class NEONGRID_API UWeaponEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponEquipmentComponent();

	/** Equip a new weapon and update AI blackboard if applicable */
	UFUNCTION(BlueprintCallable, Category="Equipment")
	void EquipWeapon(UWeaponData* NewWeapon);

	/** Get current weapon data */
	UFUNCTION(BlueprintPure, Category="Equipment")
	UWeaponData* GetCurrentWeapon() const { return CurrentWeapon; }

protected:
	virtual void BeginPlay() override;

	/** Currently equipped weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment")
	TObjectPtr<UWeaponData> CurrentWeapon;

	/** Update blackboard with weapon stats */
	void UpdateBlackboard() const;
};
