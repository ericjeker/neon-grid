// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"

#include "NeonGrid/Core/Characters/NeonCharacter.h"
#include "NeonGrid/Core/NeonGridEnums.h"

#include "NeonPlayerCharacter.generated.h"

struct FInputActionValue;
/**
 * Represents a player-controlled character extending from ANeonCharacter.
 * Supports functionality for movement, input configuration, and basic actions.
 */
UCLASS()
class NEONGRID_API ANeonPlayerCharacter : public ANeonCharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ANeonPlayerCharacter();

	/** 
	 * Moves the character relative to the camera orientation.
	 * @param MovementVector The 2D vector from input (X = Right, Y = Forward)
	 */
	void Move(const FVector2D& MovementVector);

	/**
	 * Rotates the character to face a direction or location.
	 * @param LookVector The input vector for looking (if using gamepad)
	 */
	void Look(const FVector2D& LookVector);
	
	
	void Interact(const FInputActionValue& Value) const;

	/** IGenericTeamAgentInterface implementation */
	virtual FGenericTeamId GetGenericTeamId() const override
	{
		// Hard-coded as the player will never change team
		return FGenericTeamId(static_cast<uint8>(ETeamName::Player));
	}

	/** Returns the Camera Component for the Controller to perform raycasts if needed */
	class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FollowCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	class UNeonInteractionComponent* InteractionComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	class UNeonInventoryComponent* InventoryComponent;
};
