// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"

#include "BTDecorator_AbortIfTargetDead.generated.h"

UCLASS()
class NEONGRID_API UBTDecorator_AbortIfTargetDead : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_AbortIfTargetDead();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
