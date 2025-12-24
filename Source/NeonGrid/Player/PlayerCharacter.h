// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"

#include "NeonGrid/Core/NeonCharacter.h"
#include "NeonGrid/Core/NeonGridEnums.h"

#include "PlayerCharacter.generated.h"

/**
 * Represents a player-controlled character extending from ANeonCharacter.
 * Supports functionality for movement, input configuration, and basic actions.
 */
UCLASS()
class NEONGRID_API APlayerCharacter : public ANeonCharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter();

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* FollowCamera;
};
