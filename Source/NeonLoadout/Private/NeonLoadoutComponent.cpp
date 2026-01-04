// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "NeonLoadoutComponent.h"

#include "GameFramework/Character.h"
#include "NeonEquipment.h"
#include "NeonItemFragment_Equippable.h"
#include "NeonItemInstance.h"


// Sets default values for this component's properties
UNeonLoadoutComponent::UNeonLoadoutComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UNeonLoadoutComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UNeonLoadoutComponent::EquipItem(UNeonItemInstance* ItemInstance, const ENeonEquipmentSlot Slot)
{
	if (!ItemInstance) return;

	// 1. Get the Equippable Fragment from the item
	const UNeonItemFragment_Equippable* EquipFragment = ItemInstance->FindFragment<UNeonItemFragment_Equippable>();
	if (!EquipFragment || !EquipFragment->EquipmentClass)
	{
		return;
	}

	// 2. Cleanup existing item in this slot if any
	if (ANeonEquipment* OldItem = EquipmentMap.FindRef(Slot))
	{
		OldItem->Destroy();
		EquipmentMap.Remove(Slot);
	}
	
	// 3. Spawn the new Equipment Actor
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = Cast<APawn>(GetOwner());

	if (ANeonEquipment* NewEquipment = GetWorld()->SpawnActor<ANeonEquipment>(EquipFragment->EquipmentClass, SpawnParams))
	{
		// 4. Attach to Character Mesh
		if (ACharacter* CharOwner = Cast<ACharacter>(GetOwner()))
		{
			NewEquipment->AttachToComponent(CharOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, EquipFragment->AttachmentSocket);
			NewEquipment->SetActorRelativeTransform(EquipFragment->AttachmentOffset);
		}

		// 5. Save reference
		EquipmentMap.Add(Slot, NewEquipment);

		// 6. Handle Visibility (If it's the active slot, show it; otherwise hide it)
		if (Slot == CurrentSlotIndex)
		{
			NewEquipment->Equip();
		}
		else
		{
			NewEquipment->SetActorHiddenInGame(true);
			NewEquipment->SetActorEnableCollision(false);
		}
	}

}

void UNeonLoadoutComponent::SetActiveSlot(ENeonEquipmentSlot Slot)
{
	if (CurrentSlotIndex == Slot)
	{
		return;
	}
	
	// 1. Unequip/Hide the old item
	if (ANeonEquipment* OldItem = GetCurrentEquipment())
	{
		OldItem->Unequip();
		OldItem->SetActorHiddenInGame(true);
		OldItem->SetActorEnableCollision(false);
	}

	// 2. Update Index
	CurrentSlotIndex = Slot;

	// 3. Equip/Show the new item
	if (ANeonEquipment* NewItem = GetCurrentEquipment())
	{
		NewItem->SetActorHiddenInGame(false);
		NewItem->SetActorEnableCollision(true);
		NewItem->Equip();
	}
}

void UNeonLoadoutComponent::CycleEquipmentSlot()
{
	if (CurrentSlotIndex == ENeonEquipmentSlot::Primary)
	{
		SetActiveSlot(ENeonEquipmentSlot::Secondary);
	}
	else if (CurrentSlotIndex == ENeonEquipmentSlot::Secondary)
	{
		SetActiveSlot(ENeonEquipmentSlot::Primary);
	}
}

void UNeonLoadoutComponent::StartFire()
{
	if (ANeonEquipment* CurrentEquipment = GetCurrentEquipment())
	{
		CurrentEquipment->StartPrimaryAction();
	}
}

void UNeonLoadoutComponent::StartReload()
{
	if (ANeonEquipment* CurrentEquipment = GetCurrentEquipment())
	{
		// CurrentEquipment->Reload();
	}
}
