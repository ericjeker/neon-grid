// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NeonGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class NEONGRID_API UNeonGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
};
