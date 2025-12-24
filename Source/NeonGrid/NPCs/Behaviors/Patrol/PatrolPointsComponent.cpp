// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "PatrolPointsComponent.h"


// Sets default values for this component's properties
UPatrolPointsComponent::UPatrolPointsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

FVector UPatrolPointsComponent::GetNextPatrolPoint(int32& CurrentIndex, bool& bIsReversing) const
{
	if (PatrolPoints.Num() == 0)
	{
		return FVector::ZeroVector;
	}

	switch (PatrolMode)
	{
	case EPatrolMode::Sequential:
		CurrentIndex = (CurrentIndex + 1) % PatrolPoints.Num();
		break;

	case EPatrolMode::PingPong:
		{
			if (bIsReversing)
			{
				CurrentIndex--;
				if (CurrentIndex <= 0)
				{
					CurrentIndex = 0;
					bIsReversing = false;
				}
			}
			else
			{
				CurrentIndex++;
				if (CurrentIndex >= PatrolPoints.Num() - 1)
				{
					CurrentIndex = PatrolPoints.Num() - 1;
					bIsReversing = true;
				}
			}
		}
		break;

	case EPatrolMode::Random:
		CurrentIndex = FMath::RandRange(0, PatrolPoints.Num() - 1);
		break;
	}

	return GetPatrolPointWorldLocation(CurrentIndex);
}

FVector UPatrolPointsComponent::GetPatrolPointWorldLocation(int32 Index) const
{
	if (!PatrolPoints.IsValidIndex(Index))
	{
		return FVector::ZeroVector;
	}

	if (bUseLocalSpace && GetOwner())
	{
		return GetOwner()->GetActorLocation() + PatrolPoints[Index];
	}

	return PatrolPoints[Index];
}