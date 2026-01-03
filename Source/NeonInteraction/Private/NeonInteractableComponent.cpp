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
	HandleInteract(InstigatorPawn);

	// 2. Notify the Owning Actor (or anyone else listening)
	OnInteractDelegate.Broadcast(InstigatorPawn);

	return true;
}

void UNeonInteractableComponent::HandleInteract_Implementation(APawn* InstigatorPawn)
{
	// Empty default implementation
}

void UNeonInteractableComponent::OnHoverBegin_Implementation()
{
	if (!bCanInteract)
	{
		return;
	}

	HandleHoverBegin();

	OnHoverBeginDelegate.Broadcast();
}

void UNeonInteractableComponent::HandleHoverBegin_Implementation()
{
	const AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("HoverBegin called on %s"), *Owner->GetName());
	
	// Default Behavior: Render CustomDepth (Outline)
	if (UPrimitiveComponent* RootPrim = Cast<UPrimitiveComponent>(Owner->GetRootComponent()))
	{
		RootPrim->SetRenderCustomDepth(true);
		RootPrim->SetCustomDepthStencilValue(1); 
	}
}

void UNeonInteractableComponent::OnHoverEnd_Implementation()
{
	if (!bCanInteract)
	{
		return;
	}

	HandleHoverEnd();

	OnHoverEndDelegate.Broadcast();
}

void UNeonInteractableComponent::HandleHoverEnd_Implementation()
{
	const AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("HoverEnd called on %s"), *Owner->GetName());
	
	// Default Behavior: Remove CustomDepth
	if (UPrimitiveComponent* RootPrim = Cast<UPrimitiveComponent>(Owner->GetRootComponent()))
	{
		RootPrim->SetRenderCustomDepth(false);
		RootPrim->SetCustomDepthStencilValue(0); 
	}
}
