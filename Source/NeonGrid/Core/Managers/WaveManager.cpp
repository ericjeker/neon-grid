// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "WaveManager.h"

#include "Kismet/GameplayStatics.h"
#include "NeonGrid/Combat/Components/HealthComponent.h"


// Sets default values
AWaveManager::AWaveManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AWaveManager::BeginPlay()
{
	Super::BeginPlay();

	FindSpawnAndPatrolPoints();
	SpawnWave();
}

void AWaveManager::FindSpawnAndPatrolPoints()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), SpawnPointTag, FoundActors);

	SpawnPoints.Empty();
	for (AActor* Actor : FoundActors)
	{
		SpawnPoints.Add(Actor);
	}

	FoundActors.Empty();
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), PatrolPointTag, FoundActors);

	PatrolPoints.Empty();
	for (AActor* Actor : FoundActors)
	{
		PatrolPoints.Add(Actor);
	}

	UE_LOG(LogTemp, Log, TEXT("WaveManager found %d spawn points and %d patrol points"), SpawnPoints.Num(),
	       PatrolPoints.Num());
}

void AWaveManager::CheckAndSpawnWave()
{
	// Clean up any invalid references
	AliveNPCs.RemoveAll([](const TWeakObjectPtr<AActor>& NPC)
	{
		return !NPC.IsValid();
	});

	// If no NPCs alive, spawn a new wave
	if (AliveNPCs.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("All NPCs defeated! Spawning new wave..."));
		SpawnWave();
	}
}

void AWaveManager::SpawnWave()
{
	if (!NPCClass || SpawnPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("WaveManager: Missing NPCClass or SpawnPoints!"));
		return;
	}

	CurrentWaveNumber++;

	// Spawn more enemies each wave
	const int32 EnemiesThisWave = NPCsPerWave + (CurrentWaveNumber - 1);
	UE_LOG(LogTemp, Log, TEXT("Spawning Wave %d with %d NPCs"), CurrentWaveNumber, EnemiesThisWave);

	for (int32 i = 0; i < EnemiesThisWave; i++)
	{
		// Pick a random spawn point and check if valid
		TWeakObjectPtr<AActor> SpawnPointPtr = SpawnPoints[FMath::RandRange(0, SpawnPoints.Num() - 1)];

		if (!SpawnPointPtr.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Spawn point is no longer valid, skipping..."));
			continue;
		}

		AActor* SpawnPoint = SpawnPointPtr.Get();

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AActor* NewNPC = GetWorld()->SpawnActor<AActor>(
			NPCClass,
			SpawnPoint->GetActorLocation(),
			SpawnPoint->GetActorRotation(),
			SpawnParams
		);

		if (NewNPC)
		{
			AliveNPCs.Add(NewNPC);

			// Optional: Assign a random patrol point
			if (PatrolPoints.Num() > 0)
			{
				TWeakObjectPtr<AActor> PatrolPointPtr = PatrolPoints[FMath::RandRange(0, PatrolPoints.Num() - 1)];

				if (PatrolPointPtr.IsValid())
				{
					AActor* TargetPatrol = PatrolPointPtr.Get();
					// Assign to NPC's AI controller via interface or event
				}
			}

			// Subscribe to this NPC's death
			if (UHealthComponent* HealthComp = NewNPC->FindComponentByClass<UHealthComponent>())
			{
				HealthComp->OnActorDied.AddDynamic(this, &AWaveManager::RegisterNPCDeath);
			}
		}
	}
}

void AWaveManager::RegisterNPCDeath(AActor* DeadNPC)
{
	if (DeadNPC)
	{
		AliveNPCs.Remove(DeadNPC);
		UE_LOG(LogTemp, Log, TEXT("NPC died. Remaining: %d"), AliveNPCs.Num());

		CheckAndSpawnWave();
	}
}

void AWaveManager::OnNPCDeath_Implementation(AActor* DeadNPC)
{
}
