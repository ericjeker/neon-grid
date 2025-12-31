// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "NeonInventoryItemDefinition.h"
#include "NeonInventoryTypes.generated.h"

USTRUCT(BlueprintType)
struct FNeonInventorySlot
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNeonInventoryItemDefinition> ItemDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;

	// Default constructor
	FNeonInventorySlot() : ItemDefinition(nullptr), Quantity(0) {}
	
	// Constructor with parameters to allow brace initialization
	FNeonInventorySlot(UNeonInventoryItemDefinition* InItem, int32 InQuantity)
		: ItemDefinition(InItem), Quantity(InQuantity) {}
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
