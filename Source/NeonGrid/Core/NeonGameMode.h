// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "NeonGameMode.generated.h"

/**
 * 
 */
UCLASS()
class NEONGRID_API ANeonGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	void OnNPCKilled(int32 ScoreValue) const;
	
protected:
	virtual void BeginPlay() override;
	
	void StartWaveSystem() const;
	bool bDoubleScoreActive = false;
};
