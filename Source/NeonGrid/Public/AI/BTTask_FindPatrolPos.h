// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindPatrolPos.generated.h"

/**
 * Represents a behavior tree task responsible for determining a patrol position
 * for an AI agent. The task calculates a new position within a specified radius
 * and updates the corresponding key in the AI's blackboard.
 */
UCLASS()
class NEONGRID_API UBTTask_FindPatrolPos : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FindPatrolPos();

	/**
	 * Executes the task to find a patrol position for an AI agent.
	 * 
	 * @param OwnerComp Reference to the behavior tree component that owns this task.
	 * @param NodeMemory Pointer to the allocated memory block for this node.
	 * @return The result of the task execution, represented by EBTNodeResult::Type. Typically, a Success, Failure, or InProgress value.
	 */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetLocationKey;

	UPROPERTY(EditAnywhere, Category = "AI")
	float SearchRadius = 1000.f;
};
