// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NeonInventoryTypes.h"
#include "Components/ActorComponent.h"
#include "NeonInventoryComponent.generated.h"

// Forward declarations
class UInventoryItemDefinition;
struct FNeonInventorySlot;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);

/**
 * A component that manages the inventory system, including item addition, removal,
 * weight capacity, and inventory state.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NEONINVENTORY_API UNeonInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UNeonInventoryComponent();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnInventoryChanged OnInventoryChanged;

	/** Returns true if the item was successfully added */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	ENeonInventoryResult AddItem(UNeonInventoryItemDefinition* Item, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	ENeonInventoryResult RemoveItem(UNeonInventoryItemDefinition* Item, int32 Quantity);

	UFUNCTION(BlueprintPure, Category = "Inventory")
	float GetCurrentWeightKg() const;
	
	UFUNCTION(BlueprintPure, Category = "Inventory")
	float GetMaxWeightKg() const { return MaxWeightCapacityKg; }
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetMaxWeightKg(const float NewMaxWeight) { MaxWeightCapacityKg = NewMaxWeight; };

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void EmptyInventory() { InventoryContent.Empty(); };
	
protected:
	// The actual inventory storage
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FNeonInventorySlot> InventoryContent;

	// Configuration: Max weight in kg
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
	float MaxWeightCapacityKg = 50.0f;

};
