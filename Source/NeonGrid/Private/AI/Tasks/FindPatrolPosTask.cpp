// Copyright Eric Jeker, Inc. All Rights Reserved.

#include "AI/Tasks/FindPatrolPosTask.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "NeonGridEnums.h"
#include "BehaviorTree/BlackboardComponent.h"

UFindPatrolPosTask::UFindPatrolPosTask()
{
	NodeName = "Find Patrol Position";
}

EBTNodeResult::Type UFindPatrolPosTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;

	const UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSys) return EBTNodeResult::Failed;

	FVector SearchOrigin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(NeonGridAIKeys::OriginLocation);
	const bool bUseOriginLocation = OwnerComp.GetBlackboardComponent()->GetValueAsBool(NeonGridAIKeys::ShouldPatrolFromOrigin);
	
	// If OriginLocation isn't set, or we should not use it, fall back to current
	if (!bUseOriginLocation || SearchOrigin.IsZero())
	{
		SearchOrigin = Pawn->GetActorLocation();
	}

	if (FNavLocation NextLocation; NavSys->GetRandomReachablePointInRadius(SearchOrigin, SearchRadius, NextLocation))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(NeonGridAIKeys::PatrolLocation, NextLocation.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}