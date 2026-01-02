// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NeonInteractionComponent.generated.h"


UCLASS(ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent))
class NEONINTERACTION_API UNeonInteractionComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UNeonInteractionComponent();
	
	/** Search radius for finding interactable objects */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float SearchRadius = 300.0f;
	
	/** Return the closest actor implementing the UNeonInteractable interface */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	AActor* GetClosestInteractable();
	
	/** Return the interactable actor currently under the mouse cursor */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	AActor* GetInteractableUnderCursor(bool bCheckLineOfSight);

	/** Helper to check if an actor is valid for interaction */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool IsActorInteractable(const AActor* Actor) const;
	
	/** Interact with the specified target actor if it implements the UNeonInteractable interface */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool Interact(AActor* TargetActor);
};
