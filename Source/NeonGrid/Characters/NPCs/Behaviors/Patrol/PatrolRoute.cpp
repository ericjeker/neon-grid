// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "PatrolRoute.h"


// Sets default values
APatrolRoute::APatrolRoute()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

#if WITH_EDITOR
void APatrolRoute::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
