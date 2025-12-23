// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "Characters/NonPlayerCharacter.h"

#include "AI/NonPlayerController.h"


// Sets default values
ANonPlayerCharacter::ANonPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Automatically add a non-player controller to the character
	AIControllerClass = ANonPlayerController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

/**
 * Called when the game starts or when spawned
 * Initializes the character and its AI controller.
 */
void ANonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ANonPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ANonPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

