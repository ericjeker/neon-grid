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
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UNeonInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Every frame, check what is under the cursor
	if (AActor* ActorUnderCursor = GetInteractableUnderCursor(true))
	{
		// Update the focus state
		SetHoveredActor(ActorUnderCursor);
	}
	else
	{
		SetHoveredActor(nullptr);
	}
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

	// Get all overlapping actors within the search radius
	TArray<FOverlapResult> HitResults;
	const FVector OwnerLocation = Owner->GetActorLocation();
	const FCollisionShape Sphere = FCollisionShape::MakeSphere(SearchRadius);

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

		if (!IsActorInteractable(Actor))
		{
			continue;
		}

		// Calculate the distance between the Owner and the Actor 
		const float DistanceSq = FVector::DistSquared(OwnerLocation, Actor->GetActorLocation());
		if (DistanceSq < ClosestDistanceSq)
		{
			ClosestDistanceSq = DistanceSq;
			ClosestActor = Actor;
		}
	}

	return ClosestActor;
}

AActor* UNeonInteractionComponent::GetInteractableUnderCursor(const bool bCheckLineOfSight)
{
	const AActor* Owner = GetOwner();
	if (!Owner)
	{
		return nullptr;
	}

	const APawn* OwnerPawn = Cast<APawn>(Owner);
	if (!OwnerPawn)
	{
		return nullptr;
	}

	// We need the PlayerController to access the mouse position
	const APlayerController* PC = Cast<APlayerController>(OwnerPawn->GetController());
	if (!PC)
	{
		return nullptr;
	}

	FHitResult HitResult;
	// Trace against Visibility channel (standard for mouse picking)
	// TODO: make the TraceChannel configurable
	if (PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
	{
		AActor* HitActor = HitResult.GetActor();

		// Validate the actor using our shared logic
		if (HitActor && HitActor != Owner && IsActorInteractable(HitActor))
		{
			if (bCheckLineOfSight)
			{
				// Ensure the Pawn can actually "see" the target (prevents interacting through walls)
				FHitResult LoSResult;
				FCollisionQueryParams LoSParams;
				LoSParams.AddIgnoredActor(Owner);

				const FVector StartLocation = OwnerPawn->GetActorLocation();
				const FVector EndLocation = HitResult.ImpactPoint;

				// Trace from the Character to the point the mouse clicked
				const bool bHitOcclusion = GetWorld()->LineTraceSingleByChannel(
					LoSResult,
					StartLocation,
					EndLocation,
					ECC_Visibility, // Match the visibility channel used by cursor trace
					LoSParams
				);

				// If we hit something that ISN'T our target, we are blocked
				if (bHitOcclusion && LoSResult.GetActor() != HitActor)
				{
					return nullptr;
				}
			}

			// Enforce range check even for cursor interaction
			const float DistanceSq = FVector::DistSquared(Owner->GetActorLocation(), HitActor->GetActorLocation());
			if (DistanceSq <= SearchRadius * SearchRadius)
			{
				return HitActor;
			}
		}
	}

	return nullptr;
}

bool UNeonInteractionComponent::IsActorInteractable(const AActor* Actor) const
{
	if (!Actor)
	{
		return false;
	}

	// 1. The Actor itself implements the interface OR
	if (Actor->Implements<UNeonInteractableInterface>())
	{
		return true;
	}

	// 2. The Actor has a component that implements the interface
	return (Actor->FindComponentByClass<UNeonInteractableComponent>() != nullptr);
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
	if (UActorComponent* InteractableComp = TargetActor->FindComponentByInterface(
		UNeonInteractableInterface::StaticClass()))
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

void UNeonInteractionComponent::SetHoveredActor(AActor* NewActor)
{
	// If the focus hasn't changed, do nothing
	if (NewActor == CurrentHoveredActor)
	{
		return;
	}

	// 1. Unhighlight the OLD actor
	if (CurrentHoveredActor)
	{
		if (CurrentHoveredActor->Implements<UNeonInteractableInterface>())
		{
			INeonInteractableInterface::Execute_OnHoverEnd(CurrentHoveredActor);
		}
		else if (UActorComponent* InteractableComp = CurrentHoveredActor->FindComponentByInterface(
			UNeonInteractableInterface::StaticClass()))
		{
			INeonInteractableInterface::Execute_OnHoverEnd(InteractableComp);
		}
	}

	// 2. Highlight the NEW actor
	if (NewActor)
	{
		if (NewActor->Implements<UNeonInteractableInterface>())
		{
			INeonInteractableInterface::Execute_OnHoverBegin(NewActor);
		} else if (UActorComponent* InteractableComp = NewActor->FindComponentByInterface(
			UNeonInteractableInterface::StaticClass()))
		{
			INeonInteractableInterface::Execute_OnHoverBegin(InteractableComp);
		}
	}

	// 3. Update the reference
	CurrentHoveredActor = NewActor;
}
