// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "NeonGameState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatChangedSignature, int32, NewValue);

UCLASS()
class NEONGRID_API ANeonGameState : public AGameState
{
	GENERATED_BODY()

public:
	/** Score Management */
	void AddScore(const int32 Amount)
	{
		TotalScore += Amount;
		OnScoreChanged.Broadcast(TotalScore);
	}

	int32 GetScore() const { return TotalScore; }

	void SetScore(const int32 NewScore)
	{
		TotalScore = NewScore;
		OnScoreChanged.Broadcast(TotalScore);
	}

	/** Wave Management */
	void SetWave(const int32 NewWave)
	{
		CurrentWave = NewWave;
		OnWaveChanged.Broadcast(CurrentWave);
	}

	int32 GetCurrentWave() const { return CurrentWave; }

	int NextWave()
	{
		++CurrentWave;
		OnWaveChanged.Broadcast(CurrentWave);
		return CurrentWave;
	}

	/** Enemy Management */
	int32 GetEnemiesRemaining() const { return EnemiesRemaining; }

	void SetEnemiesRemaining(const int32 NewEnemiesCount)
	{
		EnemiesRemaining = NewEnemiesCount;
		OnEnemiesRemainingChanged.Broadcast(EnemiesRemaining);
	}

	/** Multi-cast delegates */
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnStatChangedSignature OnScoreChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnStatChangedSignature OnWaveChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnStatChangedSignature OnEnemiesRemainingChanged;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Score")
	int32 TotalScore = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Wave")
	int32 CurrentWave = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Wave")
	int32 EnemiesRemaining = 0;
};
