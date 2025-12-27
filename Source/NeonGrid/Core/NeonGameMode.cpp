// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "NeonGameMode.h"

#include "NeonGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/WaveManager.h"

void ANeonGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	// 1. Start the Cinematic (e.g., via Level Sequence)
	// 2. On Cinematic finished, call StartWaveSystem()
	StartWaveSystem();
}

void ANeonGameMode::OnNPCKilled(int32 ScoreValue) const
{
	// Apply global modifiers here
	if (bDoubleScoreActive) { ScoreValue *= 2; }

	if (ANeonGameState* GS = GetGameState<ANeonGameState>())
	{
		GS->AddScore(ScoreValue);
	}
}

void ANeonGameMode::StartWaveSystem() const
{
	if (ANeonGameState* GS = Cast<ANeonGameState>(GetWorld()->GetGameState()))
	{
		GS->SetWave(0);
		GS->SetScore(0);
	}
	
	// Find our WaveManager and tell it to go!
	if (AWaveManager* WaveMgr = Cast<AWaveManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWaveManager::StaticClass())))
	{
		WaveMgr->SpawnWave(); 
	}
}
