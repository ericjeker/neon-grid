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
	
	/** Interact with the specified target actor if it implements the UNeonInteractable interface */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool Interact(AActor* TargetActor);
};
