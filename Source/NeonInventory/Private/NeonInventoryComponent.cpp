// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "NeonInventoryComponent.h"
#include "NeonInventoryItemDefinition.h"
#include "NeonInventoryTypes.h"


// Sets default values for this component's properties
UNeonInventoryComponent::UNeonInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

ENeonInventoryResult UNeonInventoryComponent::AddItem(UNeonInventoryItemDefinition* Item, int32 Quantity)
{
	if (!Item || Quantity <= 0)
	{
		return ENeonInventoryResult::Failed_InvalidItem;
	}
	
	// Check if we have enough weight capacity
	const float AvailableWeight = MaxWeightCapacityKg - GetCurrentWeightKg();
	if (AvailableWeight <= 0.0f || Item->MassKg * Quantity > AvailableWeight)
	{
		return ENeonInventoryResult::Failed_InsufficientWeightCapacity;
	}
	
	// Check if the item already exists 
	for (FNeonInventorySlot& Slot : InventoryContent)
	{
		if (Slot.ItemDefinition == Item)
		{
			// Check if we have reached the max stack size
			const int32 SpaceLeftInStack = Slot.ItemDefinition->MaxStackSize - Slot.Quantity;
			if (SpaceLeftInStack > 0)
			{
				// Add what we can to the stack
				const int32 AmountToAdd = FMath::Min(Quantity, SpaceLeftInStack);
				Slot.Quantity += AmountToAdd;
				Quantity -= AmountToAdd;
				
				if (Quantity <= 0)
				{
					OnInventoryChanged.Broadcast();
					return ENeonInventoryResult::Success;
				}
			}
		}
	}
	
	// Add the remaining quantity in new slots
	while (Quantity > 0)
	{
		const int32 AmountForNewSlot = FMath::Min(Quantity, Item->MaxStackSize);
		InventoryContent.Add({ Item, AmountForNewSlot });
		Quantity -= AmountForNewSlot;
	}
	
	OnInventoryChanged.Broadcast();
	
	return ENeonInventoryResult::Success;
}

ENeonInventoryResult UNeonInventoryComponent::RemoveItem(UNeonInventoryItemDefinition* Item, int32 Quantity)
{
	if (!Item || Quantity <= 0)
	{
		return ENeonInventoryResult::Failed_InvalidItem;
	}
	
	// Check if we have enough total quantity
	int32 TotalAvailable = 0;
	for (const FNeonInventorySlot& Slot : InventoryContent)
	{
		if (Slot.ItemDefinition == Item)
		{
			TotalAvailable += Slot.Quantity;
		}
	}
	
	if (TotalAvailable < Quantity)
	{
		return ENeonInventoryResult::Failed_InsufficientQuantity;
	}
	
	// Remove from slots (iterate backwards to safely remove empty slots)
	for (int32 i = InventoryContent.Num() - 1; i >= 0 && Quantity > 0; --i)
	{
		FNeonInventorySlot& Slot = InventoryContent[i];
		if (Slot.ItemDefinition == Item)
		{
			const int32 AmountToRemove = FMath::Min(Quantity, Slot.Quantity);
			Slot.Quantity -= AmountToRemove;
			Quantity -= AmountToRemove;
			
			// Remove empty slots
			if (Slot.Quantity <= 0)
			{
				InventoryContent.RemoveAt(i);
			}
		}
	}
	
	OnInventoryChanged.Broadcast();
	return ENeonInventoryResult::Success;
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
		if (Slot.ItemDefinition)
		{
			TotalWeight += Slot.ItemDefinition->MassKg * Slot.Quantity;
		}
	}
	
	return TotalWeight;
}

