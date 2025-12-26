// Copyright Eric Jeker, Inc. All Rights Reserved.

#include "BTTask_RotateToFaceTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"

UBTTask_RotateToFaceTarget::UBTTask_RotateToFaceTarget()
{
	NodeName = "Rotate to Face Target";
	
	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_RotateToFaceTarget, TargetActorKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_RotateToFaceTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	const APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}

	// Get target from Blackboard
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
	
	if (!TargetActor)
	{
		return EBTNodeResult::Failed;
	}

	// Calculate direction to target
	FVector DirectionToTarget = TargetActor->GetActorLocation() - ControlledPawn->GetActorLocation();
	DirectionToTarget.Z = 0.f;  // Keep rotation on the horizontal plane

	if (DirectionToTarget.IsNearlyZero())
	{
		return EBTNodeResult::Failed;
	}

	// Calculate target rotation
	const FRotator TargetRotation = DirectionToTarget.Rotation();
	const FRotator CurrentRotation = ControlledPawn->GetActorRotation();

	// Check if already facing the target within an acceptable angle
	const float AngleDifference = FMath::Abs(FMath::FindDeltaAngleDegrees(CurrentRotation.Yaw, TargetRotation.Yaw));
	
	if (AngleDifference <= AcceptableAngleDegrees)
	{
		return EBTNodeResult::Succeeded;
	}

	// Set focus on target for smooth rotation
	AIController->SetFocus(TargetActor);

	return EBTNodeResult::Succeeded;
}