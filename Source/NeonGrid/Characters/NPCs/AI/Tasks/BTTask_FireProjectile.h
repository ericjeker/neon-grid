// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FireProjectile.generated.h"

/**
 * Behavior Tree Task that makes the NPC fire a projectile at the target actor.
 * Uses the character's Gameplay Ability System to trigger the fire ability.
 */
UCLASS()
class NEONGRID_API UBTTask_FireProjectile : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FireProjectile();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	/** Blackboard key for the target actor to shoot at */
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	/** Minimum distance to fire. Won't fire if too close. */
	UPROPERTY(EditAnywhere, Category = "Combat")
	float MinFireDistance = 100.f;

	/** Maximum distance to fire. Won't fire if too far. */
	UPROPERTY(EditAnywhere, Category = "Combat")
	float MaxFireDistance = 2000.f;

	/** Whether to require line of sight to the target before firing */
	UPROPERTY(EditAnywhere, Category = "Combat")
	bool bRequireLineOfSight = true;
};