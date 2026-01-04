// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"

#include "BTService_CheckCombatTimeout.generated.h"

/**
 * Service that monitors combat state and clears it after a timeout period
 * when no target is visible.
 */
UCLASS()
class NEONGRID_API UBTService_CheckCombatTimeout : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CheckCombatTimeout();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	/** Key for combat state flag */
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector IsEngagingKey;

	/** Key for target actor */
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	/** Key for time when we last saw the target */
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TimeLastSawTargetKey;

	/** How long to stay in combat after losing sight (if not set in archetype) */
	UPROPERTY(EditAnywhere, Category = "Combat")
	float DefaultCombatTimeout = 10.f;
};