#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdateLastKnownLocation.generated.h"

UCLASS()
class NEONGRID_API UBTService_UpdateLastKnownLocation : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_UpdateLastKnownLocation();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};