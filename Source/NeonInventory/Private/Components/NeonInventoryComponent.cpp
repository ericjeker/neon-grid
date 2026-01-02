// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "Components/NeonInventoryComponent.h"

#include "NeonItemDefinition.h"
#include "NeonInventoryTypes.h"
#include "NeonItemInstance.h"
#include "Fragments/NeonItemFragment_Inventory.h"


// Sets default values for this component's properties
UNeonInventoryComponent::UNeonInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

ENeonInventoryResult UNeonInventoryComponent::AddItem(UNeonItemInstance* Item, const int32 Quantity)
{
	if (!Item || Quantity <= 0)
	{
		return ENeonInventoryResult::Failed_InvalidItem;
	}

	// Check if we have enough slots
	if (InventoryContent.Num() >= MaxInventorySize)
	{
		return ENeonInventoryResult::Failed_InventoryFull;
	}

	const UNeonItemFragment_Inventory* InventoryFragment = Item->FindFragment<UNeonItemFragment_Inventory>();
	if (!InventoryFragment)
	{
		return ENeonInventoryResult::Failed_InvalidItem;
	}

	// Check if we have enough weight capacity
	const float AvailableWeight = MaxWeightCapacityKg - GetCurrentWeightKg();
	if (AvailableWeight <= 0.0f || InventoryFragment->MassKg * Quantity > AvailableWeight)
	{
		return ENeonInventoryResult::Failed_InsufficientWeightCapacity;
	}

	// Add the item to the inventory
	InventoryContent.Add({Item, Quantity});

	// Broadcast the inventory change event
	OnInventoryChanged.Broadcast();

	return ENeonInventoryResult::Success;
}

ENeonInventoryResult UNeonInventoryComponent::RemoveItem(UNeonItemInstance* Item, const int32 Quantity)
{
	if (!Item || Quantity <= 0)
	{
		return ENeonInventoryResult::Failed_InvalidItem;
	}

	// Remove from slots (iterate backwards to safely remove empty slots)
	for (int32 i = InventoryContent.Num() - 1; i >= 0 && Quantity > 0; --i)
	{
		FNeonInventorySlot& Slot = InventoryContent[i];
		if (Slot.Item == Item)
		{
			InventoryContent.RemoveAt(i);
			OnInventoryChanged.Broadcast();
			return ENeonInventoryResult::Success;
		}
	}

	return ENeonInventoryResult::Failed_ItemNotFound;
}

float UNeonInventoryComponent::GetCurrentWeightKg() const
{
	if (InventoryContent.Num() == 0)
	{
		return 0.0f;
	}

	float TotalWeight = 0.0f;
	for (const FNeonInventorySlot& Slot : InventoryContent)
	{
		if (Slot.Item)
		{
			if (const UNeonItemFragment_Inventory* InventoryFragment = Slot.Item->FindFragment<
				UNeonItemFragment_Inventory>())
			{
				TotalWeight += InventoryFragment->MassKg * Slot.Quantity;
			}
		}
	}

	return TotalWeight;
}
