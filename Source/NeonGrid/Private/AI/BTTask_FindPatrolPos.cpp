// Copyright Eric Jeker, Inc. All Rights Reserved.

#include "AI/BTTask_FindPatrolPos.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = "Find Patrol Pos";
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSys) return EBTNodeResult::Failed;

	FNavLocation NextLocation;
	if (NavSys->GetRandomReachablePointInRadius(Pawn->GetActorLocation(), SearchRadius, NextLocation))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(TargetLocationKey.SelectedKeyName, NextLocation.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}