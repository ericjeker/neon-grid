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

void ANeonInteractableCrate::Highlight_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Highlight called on %s"), *GetName());
	
	// Option 1: Render CustomDepth (Used for Outlines)
	if (UPrimitiveComponent* RootPrim = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		RootPrim->SetRenderCustomDepth(true);
		// 1 = usually standard outline, 2 = enemy, etc. (Depends on your PP Material)
		RootPrim->SetCustomDepthStencilValue(1); 
	}
	
	// Option 2: Simple Scale Animation ("Juice")
	// You could also play a Timeline here or set a scalar parameter on a material.
}

void ANeonInteractableCrate::Unhighlight_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Unhighlight called on %s"), *GetName());
	
	if (UPrimitiveComponent* RootPrim = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		RootPrim->SetRenderCustomDepth(false);
	}
}