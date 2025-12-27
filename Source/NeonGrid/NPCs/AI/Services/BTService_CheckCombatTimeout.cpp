// Copyright Eric Jeker, Inc. All Rights Reserved.

#include "BTService_CheckCombatTimeout.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NeonGrid/NPCs/NonPlayerCharacter.h"
#include "NeonGrid/NPCs/NPCArchetypeData.h"

UBTService_CheckCombatTimeout::UBTService_CheckCombatTimeout()
{
	NodeName = "Check Combat Timeout";
	
	// This service should tick relatively frequently to be responsive
	Interval = 0.5f;
	RandomDeviation = 0.1f;

	IsEngagingKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_CheckCombatTimeout, IsEngagingKey));
	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_CheckCombatTimeout, TargetActorKey), AActor::StaticClass());
	TimeLastSawTargetKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_CheckCombatTimeout, TimeLastSawTargetKey));
}

void UBTService_CheckCombatTimeout::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return;
	}

	// Only check timeout if we're in combat
	bool bIsInCombat = BlackboardComp->GetValueAsBool(IsEngagingKey.SelectedKeyName);
	if (!bIsInCombat)
	{
		return;
	}

	// If we can see the target, don't timeout
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (TargetActor)
	{
		return;  // Still have a target, stay in combat
	}

	// Get the timeout value from archetype
	float CombatTimeout = DefaultCombatTimeout;
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		if (ANonPlayerCharacter* NPC = Cast<ANonPlayerCharacter>(AIController->GetPawn()))
		{
			if (UNPCArchetypeData* Archetype = NPC->GetArchetypeData())
			{
				CombatTimeout = Archetype->CombatTimeout;
			}
		}
	}

	// Check if timeout has elapsed
	float TimeLastSaw = BlackboardComp->GetValueAsFloat(TimeLastSawTargetKey.SelectedKeyName);
	float CurrentTime = GetWorld()->GetTimeSeconds();
	float TimeSinceLastSaw = CurrentTime - TimeLastSaw;

	if (TimeSinceLastSaw >= CombatTimeout)
	{
		// Timeout elapsed, exit combat mode
		BlackboardComp->SetValueAsBool(IsEngagingKey.SelectedKeyName, false);
		BlackboardComp->ClearValue(TimeLastSawTargetKey.SelectedKeyName);
		
		// Clear focus
		if (AIController)
		{
			AIController->ClearFocus(EAIFocusPriority::Gameplay);
		}

		UE_LOG(LogTemp, Log, TEXT("Combat timeout elapsed. Returning to normal behavior."));
	}
}