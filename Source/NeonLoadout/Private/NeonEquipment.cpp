// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "NeonEquipment.h"


// Sets default values
ANeonEquipment::ANeonEquipment()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	EquipmentMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipmentMesh"));
	SetRootComponent(EquipmentMesh);
}

// Called when the game starts or when spawned
void ANeonEquipment::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANeonEquipment::Equip()
{
}

void ANeonEquipment::Unequip()
{
}

void ANeonEquipment::StartPrimaryAction_Implementation()
{
}

void ANeonEquipment::StopPrimaryAction_Implementation()
{
}
