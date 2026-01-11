#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NeonEquipmentTypes.h"
#include "NeonLoadoutComponent.generated.h"

// Forward declarations
class UNeonItemDefinition;
class UNeonItemInstance;
class ANeonEquipment;

/**
 * Component responsible for managing the player's loadout, including equipping items,
 * switching active slots, and handling the functionality of weapons and equipment.
 */
UCLASS()
class NEONLOADOUT_API UNeonLoadoutComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UNeonLoadoutComponent();
	
	UFUNCTION(BlueprintCallable, Category = "Loadout")
	void EquipItem(UNeonItemInstance* ItemInstance, ENeonEquipmentSlot Slot);

	UFUNCTION(BlueprintCallable, Category = "Loadout")
	ANeonEquipment* GetCurrentEquipment() const { return EquipmentMap.FindRef(CurrentSlotIndex); }

	UFUNCTION(BlueprintCallable, Category = "Loadout")
	void SetActiveSlot(ENeonEquipmentSlot Slot);
	
	UFUNCTION(BlueprintCallable, Category = "Loadout")
	void InitializeStartingLoadout();

	/**
	 * Scroll between Primary and Secondary weapons
	 */
	UFUNCTION(BlueprintCallable, Category = "Loadout")
	void CycleEquipmentSlot(float Direction);

	/**
	 * Will fire the currently active weapon
	 */
	UFUNCTION(BlueprintCallable, Category = "Loadout")
	void StartFire();
	
	UFUNCTION(BlueprintCallable, Category = "Loadout")
	void StopFire();
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Loadout")
	TObjectPtr<UNeonItemDefinition> StartingPrimaryWeaponDefinition;
	
	UPROPERTY(EditDefaultsOnly, Category = "Loadout")
	TObjectPtr<UNeonItemDefinition> StartingSecondaryWeaponDefinition;
	
private:
	UPROPERTY()
	ENeonEquipmentSlot CurrentSlotIndex = ENeonEquipmentSlot::Primary;
	
	UPROPERTY()
	TMap<ENeonEquipmentSlot, ANeonEquipment*> EquipmentMap;
	
};
