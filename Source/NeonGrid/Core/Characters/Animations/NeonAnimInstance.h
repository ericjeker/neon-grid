// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NeonAnimInstance.generated.h"

UCLASS()
class NEONGRID_API UNeonAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// The safe place to update variables. Runs on a worker thread.
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	// Cache the Pawn owner once at startup
	virtual void NativeInitializeAnimation() override;

protected:
	// WEAK pointer to the owner. Never keep strong references (UPROPERTY) to Actors here.
	// Thread-safe way to access the owner.
	UPROPERTY(Transient)
	TObjectPtr<APawn> OwnerPawn;

	// --- ANIMATION VARIABLES ---
	// These are read by the Blueprint Graph. They must be Thread Safe.

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion")
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion")
	float LocomotionDirection; // -180 to 180 degrees

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion")
	bool bIsAccelerating;
};
