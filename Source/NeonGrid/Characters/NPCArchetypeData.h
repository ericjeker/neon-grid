// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BehaviorTree/BehaviorTree.h"

#include "NeonGrid/NeonGridEnums.h"

#include "NPCArchetypeData.generated.h"

UCLASS(BlueprintType)
class UNPCArchetypeData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Team")
	ETeamName TeamName = ETeamName::Neutrals;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Perception")
	float SightRadius = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Perception")
	float LoseSightRadius = 2500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Perception")
	float PeripheralVisionDegrees = 70.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI")
	bool bShouldPatrolFromOrigin = true;

	// stats, movement speeds, etc.
};
