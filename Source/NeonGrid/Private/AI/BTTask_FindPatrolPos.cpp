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
	const APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;

	const UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSys) return EBTNodeResult::Failed;

	FVector SearchOrigin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(OriginLocationKey.SelectedKeyName);
	const bool bUseOriginLocation = OwnerComp.GetBlackboardComponent()->GetValueAsBool(ShouldPatrolFromOriginKey.SelectedKeyName);
	
	// If OriginLocation isn't set, or we should not use it, fall back to current
	if (!bUseOriginLocation || SearchOrigin.IsZero())
	{
		SearchOrigin = Pawn->GetActorLocation();
	}

	if (FNavLocation NextLocation; NavSys->GetRandomReachablePointInRadius(SearchOrigin, SearchRadius, NextLocation))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(TargetLocationKey.SelectedKeyName, NextLocation.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}