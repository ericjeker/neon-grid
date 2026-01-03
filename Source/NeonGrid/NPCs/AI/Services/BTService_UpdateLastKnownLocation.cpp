// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "BTService_UpdateLastKnownLocation.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"
#include "NeonGrid/Core/NeonGridEnums.h"

UBTService_UpdateLastKnownLocation::UBTService_UpdateLastKnownLocation()
{
	NodeName = "Update Last Known Location";
	Interval = 0.1f; // Update twice per second
	RandomDeviation = 0.1f;
}

void UBTService_UpdateLastKnownLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return;
	}

	// Get the current target actor
	const AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(NeonGridAIKeys::TargetActor));
	if (!TargetActor || !IsValid(TargetActor))
	{
		return;
	}

	// Update the last known location to the target's current position
	const FVector CurrentTargetLocation = TargetActor->GetActorLocation();
	BlackboardComp->SetValueAsVector(NeonGridAIKeys::LastKnownLocation, CurrentTargetLocation);

	// Update the timestamp
	if (UWorld* World = OwnerComp.GetWorld())
	{
		BlackboardComp->SetValueAsFloat(NeonGridAIKeys::TimeLastSawTarget, World->GetTimeSeconds());
	}
}