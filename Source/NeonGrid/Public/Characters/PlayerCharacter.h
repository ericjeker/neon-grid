// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "InputActionValue.h"

#include "BaseCharacter.h"
#include "NeonGridEnums.h"

#include "PlayerCharacter.generated.h"

/**
 * Represents a player-controlled character extending from ABaseCharacter.
 * Supports functionality for movement, input configuration, and basic actions.
 */
UCLASS()
class NEONGRID_API APlayerCharacter : public ABaseCharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/** IGenericTeamAgentInterface implementation */
	virtual FGenericTeamId GetGenericTeamId() const override
	{
		// Hard-coded as the player will never change team
		return FGenericTeamId(static_cast<uint8>(ETeamName::Player));
	}

	/** Actions */
	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	void Fire();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	/** Actions */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float FireRate = 0.3f;

private:
	// State for the fire rate calculation
	float LastFireTime = 0.0f;
};
