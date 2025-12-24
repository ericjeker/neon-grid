// Copyright Eric Jeker, Inc. All Rights Reserved.

#include "FindPatrolPosTask.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NeonGrid/NeonGridEnums.h"
#include "NeonGrid/Components/PatrolPointsComponent.h"

UFindPatrolPosTask::UFindPatrolPosTask()
{
	NodeName = "Find Patrol Position";
}

EBTNodeResult::Type UFindPatrolPosTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return EBTNodeResult::Failed;
	
	// Try to use patrol points first if enabled
	if (bUsePatrolPoints)
	{
		// The PatrolPointsComponent is, for now, attached to the pawn
		const UPatrolPointsComponent* PatrolComp = Pawn->FindComponentByClass<UPatrolPointsComponent>();
		if (PatrolComp && PatrolComp->GetPatrolPointCount() > 0)
		{
			int32 CurrentIndex = BlackboardComp->GetValueAsInt(NeonGridAIKeys::PatrolPointIndex);
			bool bIsReversing = BlackboardComp->GetValueAsBool(NeonGridAIKeys::IsReversingPatrol);

			const FVector NextLocation = PatrolComp->GetNextPatrolPoint(CurrentIndex, bIsReversing);

			BlackboardComp->SetValueAsInt(NeonGridAIKeys::PatrolPointIndex, CurrentIndex);
			BlackboardComp->SetValueAsBool(NeonGridAIKeys::IsReversingPatrol, bIsReversing);
			BlackboardComp->SetValueAsVector(NeonGridAIKeys::PatrolLocation, NextLocation);

			return EBTNodeResult::Succeeded;
		}
	}

	// Fall back to random patrol
	const UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSys) return EBTNodeResult::Failed;

	FVector SearchOrigin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(NeonGridAIKeys::OriginLocation);
	const bool bUseOriginLocation = OwnerComp.GetBlackboardComponent()->GetValueAsBool(
		NeonGridAIKeys::ShouldPatrolFromOrigin);

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
