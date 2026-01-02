// Copyright Eric Jeker, Inc. All Rights Reserved.

#include "NeonInteractableComponent.h"


// Sets default values for this component's properties
UNeonInteractableComponent::UNeonInteractableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

bool UNeonInteractableComponent::TryInteract_Implementation(APawn* InstigatorPawn)
{
	if (!bCanInteract)
	{
		return false;
	}

	if (!InstigatorPawn)
	{
		return false;
	}

	// 1. Notify the Component's own Blueprint logic (if any)
	OnInteracted(InstigatorPawn);

	// 2. Notify the Owning Actor (or anyone else listening)
	OnInteractDelegate.Broadcast(InstigatorPawn);

	return true; 
}

void UNeonInteractableComponent::OnInteracted_Implementation(APawn* InstigatorPawn)
{
	// Empty default implementation
}