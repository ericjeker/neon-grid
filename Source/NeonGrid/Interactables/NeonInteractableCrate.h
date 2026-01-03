// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NeonInteractableInterface.h"
#include "NeonInteractableCrate.generated.h"

UCLASS()
class NEONGRID_API ANeonInteractableCrate : public AActor, public INeonInteractableInterface
{
	GENERATED_BODY()
	
public:	
	ANeonInteractableCrate();
	
	// INeonInteractableInterface implementations
	virtual bool TryInteract_Implementation(APawn* InstigatorPawn) override;
	virtual void OnHoverBegin_Implementation() override;
	virtual void OnHoverEnd_Implementation() override;

protected:
	/** 
	 * Event triggered when interaction is validated.
	 * Use this in the Event Graph to define behavior (Play Sound, Open Animation, etc).
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction", meta = (DisplayName = "On Crate Interacted"))
	void OnCrateInteracted(APawn* InstigatorPawn);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FText InteractionPrompt = FText::FromString("Press E to open crate");
};
