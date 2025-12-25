// Copyright Eric Jeker, Inc. All Rights Reserved.

#include "BTTask_FindPatrolPosition.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NeonGrid/Core/NeonGridEnums.h"
#include "NeonGrid/NPCs/Behaviors/Patrol/PatrolPointsComponent.h"

UBTTask_FindPatrolPosition::UBTTask_FindPatrolPosition()
{
	NodeName = "Find Patrol Position";
}

EBTNodeResult::Type UBTTask_FindPatrolPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC) return EBTNodeResult::Failed;
	
	const APawn* Pawn = AIC->GetPawn();
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

	FVector SearchOrigin = BlackboardComp->GetValueAsVector(NeonGridAIKeys::OriginLocation);
	const bool bUseOriginLocation = BlackboardComp->GetValueAsBool(
		NeonGridAIKeys::ShouldPatrolFromOrigin);

	// If OriginLocation isn't set, or we should not use it, fall back to current position
	if (!bUseOriginLocation || SearchOrigin.IsZero())
	{
		SearchOrigin = Pawn->GetActorLocation();
	}

	if (FNavLocation NextLocation; NavSys->GetRandomReachablePointInRadius(SearchOrigin, SearchRadius, NextLocation))
	{
		BlackboardComp->SetValueAsVector(NeonGridAIKeys::PatrolLocation, NextLocation.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
