// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NeonInteractableInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UNeonInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class INeonInteractableInterface : public IInterface
{
	/**
	 * Generates Execute_TryInteract and TryInteract_Implementation
	 */
	GENERATED_BODY()

public:
	/** 
	 * Validation function to know if the event can be called 
	 * 
	 * - Is the crate already open? (Return false)
	 * - Does the player have the required key? (Return false)
	 * - Is the crate currently being destroyed? (Return false)
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	bool TryInteract(APawn* InstigatorPawn);
};
