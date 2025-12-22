// Copyright Eric Jeker, Inc. All Rights Reserved.

#include "AI/NonPlayerController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/NonPlayerCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GenericTeamAgentInterface.h"

// Sets default values
ANonPlayerController::ANonPlayerController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	// Configure Sight Sense
	SightConfig->SightRadius = 2000.f;
	SightConfig->LoseSightRadius = 2500.f;
	SightConfig->PeripheralVisionAngleDegrees = 90.f;
	// Relative to the agent, not the player
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;

	// Assign config to component
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
}

// Called when the game starts or when spawned
void ANonPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Bind the perception update function
	if (AIPerceptionComponent)
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ANonPlayerController::OnTargetDetected);
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

	// Explicitly set the Controller's Team ID to match the Pawn's Team ID
	if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(InPawn))
	{
		SetGenericTeamId(TeamAgent->GetGenericTeamId());
	}

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
				BB->SetValueAsBool(ShouldPatrolFromOriginKeyName, NPC->ShouldPatrolFromOrigin());
			}
		}
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
			BB->SetValueAsObject(TargetActorKeyName, Actor);

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
			UE_LOG(LogTemp, Log, TEXT("Just a random: %s"), *Actor->GetName());
		}
	}
	else
	{
		const AActor* CurrentTarget = Cast<AActor>(BB->GetValueAsObject(TargetActorKeyName));
		UE_LOG(LogTemp, Warning, TEXT("Lost track of Target: %s"), *CurrentTarget->GetName());
		
		// We lost sight of a target. Was it our current target?
		if (CurrentTarget == Actor)
		{
			// We lost the actual target we were chasing. Clear the value.
			BB->SetValueAsObject(TargetActorKeyName, nullptr);
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
