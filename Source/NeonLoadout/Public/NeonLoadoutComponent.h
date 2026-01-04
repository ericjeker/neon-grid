#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NeonEquipmentTypes.h"
#include "NeonLoadoutComponent.generated.h"

// Forward declarations
class UNeonItemInstance;
class ANeonEquipment;

UCLASS()
class NEONLOADOUT_API UNeonLoadoutComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UNeonLoadoutComponent();
	
	UPROPERTY()
	ENeonEquipmentSlot CurrentSlotIndex = ENeonEquipmentSlot::Primary;
	
	UPROPERTY()
	TMap<ENeonEquipmentSlot, ANeonEquipment*> EquipmentMap;
	
	UFUNCTION(BlueprintCallable, Category = "Loadout")
	void EquipItem(UNeonItemInstance* ItemInstance, ENeonEquipmentSlot Slot);

	UFUNCTION(BlueprintCallable, Category = "Loadout")
	ANeonEquipment* GetCurrentEquipment() const { return EquipmentMap.FindRef(CurrentSlotIndex); }

	UFUNCTION(BlueprintCallable, Category = "Loadout")
	void SetActiveSlot(ENeonEquipmentSlot Slot);

	/**
	 * Scroll between Primary and Secondary weapons
	 */
	UFUNCTION(BlueprintCallable, Category = "Loadout")
	void CycleEquipmentSlot();

	/**
	 * Will fire the currently active weapon
	 */
	UFUNCTION(BlueprintCallable, Category = "Loadout")
	void StartFire();
	
	UFUNCTION(BlueprintCallable, Category = "Loadout")
	void StartReload();

protected:
	virtual void BeginPlay() override;
};
