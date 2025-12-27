// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ApproachTarget.generated.h"

/**
 * A behavior tree task responsible for moving an AI-controlled character toward
 * a specified target. The task uses weapon range or fallback values to determine
 * the optimal distance from the target.
 */
UCLASS()
class NEONGRID_API UBTTask_ApproachTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ApproachTarget();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	/** Target to approach */
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	/** Use weapon's optimal range from blackboard */
	UPROPERTY(EditAnywhere, Category = "Movement")
	bool bUseWeaponOptimalRange = true;

	/** Fallback if weapon data not available */
	UPROPERTY(EditAnywhere, Category = "Movement")
	float FallbackRange = 800.f;

	/** Acceptable radius around optimal range */
	UPROPERTY(EditAnywhere, Category = "Movement")
	float RangeTolerance = 100.f;
};
