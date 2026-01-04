// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "BTDecorator_AbortIfTargetDead.h"

UBTDecorator_AbortIfTargetDead::UBTDecorator_AbortIfTargetDead()
{
}

bool UBTDecorator_AbortIfTargetDead::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
                                                                uint8* NodeMemory) const
{
	return Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
}
