// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "NeonBTTask_ClearFocus.h"

#include "AIController.h"


UNeonBTTask_ClearFocus::UNeonBTTask_ClearFocus()
{
	NodeName = "Clear Focus";
}

EBTNodeResult::Type UNeonBTTask_ClearFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}
	
	AIController->ClearFocus(EAIFocusPriority::Gameplay);
	return EBTNodeResult::Succeeded;
}
