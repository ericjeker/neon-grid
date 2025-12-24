// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "NonPlayerCharacter.h"

#include "NeonGrid/AI/NonPlayerController.h"


// Sets default values
ANonPlayerCharacter::ANonPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize the components
	PatrolPointsComponent = CreateDefaultSubobject<UPatrolPointsComponent>(TEXT("PatrolPointsComponent"));

	// Automatically add a non-player controller to the character
	AIControllerClass = ANonPlayerController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	TeamID = FGenericTeamId(static_cast<uint8>(ETeamName::Neutrals));
}

/**
 * Called when the game starts or when spawned
 * Initializes the character and its AI controller.
 */
void ANonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (ArchetypeData)
	{
		TeamID = FGenericTeamId(static_cast<uint8>(ArchetypeData->TeamName));
	}
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

