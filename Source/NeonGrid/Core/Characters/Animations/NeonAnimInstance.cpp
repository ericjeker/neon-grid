// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "NeonAnimInstance.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

void UNeonAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwnerPawn = TryGetPawnOwner();
}

void UNeonAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (!OwnerPawn)
	{
		return; // No owner, nothing to update
	}

	// 1. GET VELOCITY
	// We access the movement component safely
	FVector Velocity = OwnerPawn->GetVelocity();

	// 2. CALCULATE SPEED (Metric: cm/s)
	// We ignore Z (jumping/falling) for top-down movement usually
	GroundSpeed = FVector(Velocity.X, Velocity.Y, 0.0f).Size();

	// 3. CALCULATE LOCAL DIRECTION
	// Relative to where the actor is facing.
	// 0 = Forward, 90 = Right, -90 = Left, 180 = Backward
	LocomotionDirection = UKismetAnimationLibrary::CalculateDirection(Velocity, OwnerPawn->GetActorRotation());

	// 4. ACCELERATION CHECK (Optional but useful)
	// If you need to know if the player is pressing keys vs just sliding to a stop.
	// (Requires casting to your specific character class or using an interface, 
	// simplified here for clarity).
	// bIsAccelerating = MyCharacter->GetCharacterMovement()->GetCurrentAcceleration().SizeSquared() > 0.0f;
}
