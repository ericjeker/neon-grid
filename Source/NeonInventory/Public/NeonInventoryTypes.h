// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "NeonItemInstance.h"
#include "NeonInventoryTypes.generated.h"

USTRUCT(BlueprintType)
struct FNeonInventorySlot
{
	GENERATED_BODY()
	
	/** The inventory slot contains an item instance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNeonItemInstance> Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;

	// Default constructor
	FNeonInventorySlot() : Item(nullptr), Quantity(0) {}
	
	// Constructor with parameters to allow brace initialization
	FNeonInventorySlot(UNeonItemInstance* InItem, const int32 InQuantity)
		: Item(InItem), Quantity(InQuantity) {}
};

UENUM(BlueprintType)
enum class ENeonInventoryResult : uint8
{
	Success,
	Failed_InventoryFull,
	Failed_InvalidItem,
	Failed_InsufficientQuantity, 
	Failed_ItemNotFound, 
	Failed_InsufficientWeightCapacity
};
