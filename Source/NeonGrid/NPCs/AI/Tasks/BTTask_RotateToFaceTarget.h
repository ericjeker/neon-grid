// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RotateToFaceTarget.generated.h"

/**
 * Behavior Tree Task that rotates the AI to face a target actor.
 * Useful for interrupting patrol to face an attacker.
 */
UCLASS()
class NEONGRID_API UBTTask_RotateToFaceTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_RotateToFaceTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	/** Blackboard key for the target to face */
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	/** How precisely the AI needs to face the target (in degrees) */
	UPROPERTY(EditAnywhere, Category = "Rotation")
	float AcceptableAngleDegrees = 10.f;
};