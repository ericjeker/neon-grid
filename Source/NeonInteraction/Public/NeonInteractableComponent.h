// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NeonInteractableInterface.h"
#include "Components/ActorComponent.h"
#include "NeonInteractableComponent.generated.h"

// Declare the delegate signature (OneParam: The pawn who instigated)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNeonInteractSignature, APawn*, InstigatorPawn);

// Declare a delegate signature for hover events (No params)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNeonHoverSignature);

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

	/** Called when the interaction system starts hovering over this object */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FNeonHoverSignature OnHoverBeginDelegate;

	/** Called when the interaction system stops hovering away from this object */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FNeonHoverSignature OnHoverEndDelegate;

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
	void HandleInteract(APawn* InstigatorPawn);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void HandleHoverBegin();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void HandleHoverEnd();

public:
	// INeonInteractableInterface implementation
	virtual bool TryInteract_Implementation(APawn* InstigatorPawn) override;
	virtual void OnHoverBegin_Implementation() override;
	virtual void OnHoverEnd_Implementation() override;

};
