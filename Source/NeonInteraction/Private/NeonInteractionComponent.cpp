// Copyright Eric Jeker, Inc. All Rights Reserved.

#include "NeonInteractionComponent.h"

#include "NeonInteractableComponent.h"
#include "Engine/OverlapResult.h"

#include "NeonInteractableInterface.h"


// Sets default values for this component's properties
UNeonInteractionComponent::UNeonInteractionComponent()
{
	// Set this component to be initialized when the game starts and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

AActor* UNeonInteractionComponent::GetClosestInteractable()
{
	const AActor* Owner = GetOwner();
	if (!Owner)
	{
		return nullptr;
	}

	const UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}

	// Get all overlapping actors within 3 meters
	TArray<FOverlapResult> HitResults;
	const FVector OwnerLocation = Owner->GetActorLocation();
	FCollisionShape Sphere = FCollisionShape::MakeSphere(SearchRadius); // TODO: radius configurable

	// Perform the overlap query
	World->OverlapMultiByChannel(
		HitResults,
		OwnerLocation,
		FQuat::Identity,
		ECC_Pawn, // TODO: make collision channel configurable
		Sphere
	);

	// Find closest interactable
	AActor* ClosestActor = nullptr;
	float ClosestDistanceSq = FLT_MAX;

	for (const FOverlapResult& Result : HitResults)
	{
		AActor* Actor = Result.GetActor();
		if (!Actor || Actor == Owner)
		{
			continue;
		}
		
		// We accept the actor if:
		// 1. The Actor itself implements the interface OR
		// 2. The Actor has a component that implements the interface
		bool bIsInteractable = Actor->Implements<UNeonInteractableInterface>();
		
		if (!bIsInteractable)
		{
			// Check for the interactable component
			bIsInteractable = (Actor->FindComponentByClass<UNeonInteractableComponent>() != nullptr);
		}

		if (!bIsInteractable)
		{
			continue;
		}
		
		// Calculate the distance between the Owner and the Actor 
		UE_LOG(LogTemp, Warning, TEXT("Found interactable actor: %s"), *Actor->GetName());
		const float DistanceSq = FVector::DistSquared(OwnerLocation, Actor->GetActorLocation());
		if (DistanceSq < ClosestDistanceSq)
		{
			ClosestDistanceSq = DistanceSq;
			ClosestActor = Actor;
		}
	}
	
	return ClosestActor;
}

bool UNeonInteractionComponent::Interact(AActor* TargetActor)
{
	if (!TargetActor)
	{
		return false;
	}
	
	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	
	// 1. Check Component First (Composition preference)
	// This finds the first component (e.g. UNeonInteractableComponent)
	if (UActorComponent* InteractableComp = TargetActor->FindComponentByInterface(UNeonInteractableInterface::StaticClass()))
	{
		// Execute on the COMPONENT
		UE_LOG(LogTemp, Warning, TEXT("Executing interaction on component: %s"), *InteractableComp->GetName());
		return INeonInteractableInterface::Execute_TryInteract(InteractableComp, InstigatorPawn);
	}

	// 2. Check Actor Second (Legacy/Direct Implementation)
	if (TargetActor->Implements<UNeonInteractableInterface>())
	{
		// Execute on the ACTOR
		UE_LOG(LogTemp, Warning, TEXT("Executing interaction on actor: %s"), *TargetActor->GetName());
		return INeonInteractableInterface::Execute_TryInteract(TargetActor, InstigatorPawn);
	}

	return false;
}
