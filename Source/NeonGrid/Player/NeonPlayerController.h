#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"
#include "NeonGrid/Core/NeonGridEnums.h"

#include "NeonPlayerController.generated.h"

// Forward declarations
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/** Custom player controller for the NeonGrid game */
UCLASS()
class NEONGRID_API ANeonPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ANeonPlayerController();
	
	// Override to setup input bindings
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* FireAction;
	
private:
	/** Input Handlers */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	// GAS Input Handlers
	void AbilityPressed(EAbilityInputID InputId);
	void AbilityReleased(EAbilityInputID InputId);
};