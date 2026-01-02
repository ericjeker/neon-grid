// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "NeonInteractableCrate.h"


// Sets default values
ANeonInteractableCrate::ANeonInteractableCrate()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
}

bool ANeonInteractableCrate::TryInteract_Implementation(APawn* InstigatorPawn)
{
	// Base implementation - can be overridden in Blueprint
	if (!InstigatorPawn)
	{
		return false;
	}
	
	// Trigger the Blueprint Event behavior
	OnCrateInteracted(InstigatorPawn);

	// Default behavior for crate
	return true;
}