// Copyright Eric Jeker, Inc. All Rights Reserved.

#include "AI/NonPlayerController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/NonPlayerCharacter.h"

// Sets default values
ANonPlayerController::ANonPlayerController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ANonPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

/**
 * Called when this controller takes possession of a Pawn.
 * Executes the assigned behavior tree if a valid BehaviorTreeAsset is set.
 *
 * @param InPawn The Pawn being possessed by this controller.
 */
void ANonPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);

		if (UBlackboardComponent* BB = GetBlackboardComponent())
		{
			// Initialize OriginLocation
			BB->SetValueAsVector(OriginLocationKeyName, InPawn->GetActorLocation());

			// Inject the Character's setting into the Blackboard
			ANonPlayerCharacter* NPC = Cast<ANonPlayerCharacter>(InPawn);

			if (NPC)
			{
				BB->SetValueAsBool(ShouldPatrolFromOriginKeyName, NPC->bShouldPatrolFromOrigin);
			}
		}
	}
}

/**
 * Called every frame to update the state of this controller.
 * Performs any per-frame updates required by this specific non-player controller.
 *
 * @param DeltaTime The time elapsed since the last frame, in seconds.
 */
void ANonPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
