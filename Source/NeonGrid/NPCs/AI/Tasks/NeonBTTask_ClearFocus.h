// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "NeonBTTask_ClearFocus.generated.h"


UCLASS()
class NEONGRID_API UNeonBTTask_ClearFocus : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UNeonBTTask_ClearFocus();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
