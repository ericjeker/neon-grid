// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NeonInteractableInterface.h"
#include "Components/ActorComponent.h"
#include "NeonInteractableComponent.generated.h"

// Declare the delegate signature (OneParam: The pawn who instigated)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNeonInteractSignature, APawn*, InstigatorPawn);

/**
 * Component that makes any Actor interactable.
 * Automatically implements INeonInteractableInterface and forwards calls to Blueprint or overrides.
 */
UCLASS(ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent))
class NEONINTERACTION_API UNeonInteractableComponent : public UActorComponent, public INeonInteractableInterface
{
	GENERATED_BODY()

public:
	UNeonInteractableComponent();

	/**
	 * For defining contextual behavior. Use this when the Component is just a "dumb trigger", and the Actor needs to 
	 * decide what happens to the rest of the game world.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FNeonInteractSignature OnInteractDelegate;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bCanInteract = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FText InteractionPrompt = FText::FromString("Press E to interact");
	
	/** 
	 * Called when interaction occurs for defining intrinsic behavior of the component. Use this when you want to 
	 * create a specific type of interactable component that always does the same thing, regardless of which Actor it is 
	 * attached to. 
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void OnInteracted(APawn* InstigatorPawn);

public:
	// INeonInteractableInterface implementation
	virtual bool TryInteract_Implementation(APawn* InstigatorPawn) override;

};
