// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BehaviorTree/BehaviorTree.h"

#include "NeonGrid/Core/NeonGridEnums.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI|Patrol")
	bool bShouldPatrolFromOrigin = true;

	/** How long the NPC stays in combat mode after losing sight of the target */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI|Combat")
	float CombatTimeout = 10.f;

	/** 
	 * Minimum time between shots (seconds)
	 * @deprecated Use WeaponData in the Pawn to know which FireRate to use
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI|Combat")
	float FireRate = 1.5f;
};
