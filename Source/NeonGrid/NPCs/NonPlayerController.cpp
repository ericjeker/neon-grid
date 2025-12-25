// Copyright Eric Jeker, Inc. All Rights Reserved.

#include "NonPlayerController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GenericTeamAgentInterface.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "NeonGrid/NPCs/NonPlayerCharacter.h"
#include "NeonGrid/NPCs/NPCArchetypeData.h"

// Sets default values
ANonPlayerController::ANonPlayerController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialization
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	SenseSightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	// Relative to the agent, not the player
	SenseSightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SenseSightConfig->DetectionByAffiliation.bDetectNeutrals = false;
	SenseSightConfig->DetectionByAffiliation.bDetectFriendlies = false;

	// Assign config to component
	PerceptionComponent->ConfigureSense(*SenseSightConfig);
	PerceptionComponent->SetDominantSense(SenseSightConfig->GetSenseImplementation());
}

// Called when the game starts or when spawned
void ANonPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Bind the perception update function
	if (PerceptionComponent)
	{
		PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ANonPlayerController::OnTargetDetected);
	}
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

	// Validate the possessed pawn and its configuration data
	const ANonPlayerCharacter* NPC = Cast<ANonPlayerCharacter>(InPawn);
	if (!NPC)
	{
		return;
	}

	const UNPCArchetypeData* Config = NPC->GetArchetypeData();
	if (!Config)
	{
		return;
	}

	// Start Behavior Tree
	if (Config->BehaviorTree)
	{
		RunBehaviorTree(Config->BehaviorTree);

		if (const auto BB = GetBlackboardComponent())
		{
			// Initialize default values
			BB->SetValueAsVector(NeonGridAIKeys::OriginLocation, InPawn->GetActorLocation());
			BB->SetValueAsBool(NeonGridAIKeys::ShouldPatrolFromOrigin, Config->bShouldPatrolFromOrigin);
		}
	}

	// Synchronize Team ID for IGenericTeamAgentInterface
	SetGenericTeamId(FGenericTeamId(static_cast<uint8>(Config->TeamName)));

	// Configure Perception based on Archetype
	if (PerceptionComponent && SenseSightConfig)
	{
		SenseSightConfig->SightRadius = Config->SightRadius;
		SenseSightConfig->LoseSightRadius = Config->LoseSightRadius;
		SenseSightConfig->PeripheralVisionAngleDegrees = Config->PeripheralVisionDegrees;

		PerceptionComponent->ConfigureSense(*SenseSightConfig);
		PerceptionComponent->SetDominantSense(SenseSightConfig->GetSenseImplementation());
	}
}

ETeamAttitude::Type ANonPlayerController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(&Other))
	{
		// If the other actor is a different team, they are an enemy
		if (OtherTeamAgent->GetGenericTeamId() != GetGenericTeamId())
		{
			return ETeamAttitude::Hostile;
		}

		return ETeamAttitude::Friendly;
	}

	// If they are not in a team, they are neutral
	return ETeamAttitude::Neutral;
}

/**
 * Triggered when a target is detected by the AI's perception system.
 * Processes the detected stimulus and the associated actor to determine appropriate behavior.
 *
 * @param Actor The actor that was detected by the AI perception system.
 * @param Stimulus The stimulus data associated with the detection, containing information such as location and signal strength.
 */
void ANonPlayerController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!BB) { return; }

	if (Stimulus.WasSuccessfullySensed())
	{
		// Check our attitude towards the actor
		const ETeamAttitude::Type Attitude = GetTeamAttitudeTowards(*Actor);

		if (Attitude == ETeamAttitude::Hostile)
		{
			// Update Blackboard to chase this actor
			BB->SetValueAsObject(NeonGridAIKeys::TargetActor, Actor);

			// Store the last known location (where we currently see them)
			BB->SetValueAsVector(NeonGridAIKeys::LastKnownLocation, Stimulus.StimulusLocation);

			// Store the current time when seeing the target
			BB->SetValueAsFloat(NeonGridAIKeys::TimeLastSawTarget, GetWorld()->GetTimeSeconds());

			// Logic for when an ENEMY is seen (Attack, Chase, etc.)
			UE_LOG(LogTemp, Warning, TEXT("Hostile Detected: %s"), *Actor->GetName());
		}
		else if (Attitude == ETeamAttitude::Friendly)
		{
			// Logic for when a FRIEND is seen (Heal, Follow, Wave)
			UE_LOG(LogTemp, Log, TEXT("Hello, friend: %s"), *Actor->GetName());
		}
		else
		{
			// Logic for when a Neutral is seen (Ignore, Salute)
			UE_LOG(LogTemp, Log, TEXT("Just a random: %s"), *Actor->GetName());
		}
	}
	else
	{
		const AActor* CurrentTarget = Cast<AActor>(BB->GetValueAsObject(NeonGridAIKeys::TargetActor));

		// We lost sight of a target. Was it our current target?
		if (CurrentTarget && CurrentTarget == Actor)
		{
			// We lost the actual target we were chasing. Clear the value but keep the LastKnownLocation
			BB->SetValueAsObject(NeonGridAIKeys::TargetActor, nullptr);

			// Store the current time when last saw the target
			BB->SetValueAsFloat(NeonGridAIKeys::TimeLastSawTarget, GetWorld()->GetTimeSeconds());

			UE_LOG(LogTemp, Warning, TEXT("Lost sight of hostile: %s. Investigating last known location."),
			       *Actor->GetName());
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
