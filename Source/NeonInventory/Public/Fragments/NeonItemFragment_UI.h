// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "NeonItemFragment.h"
#include "NeonItemFragment_UI.generated.h"

UCLASS()
class NEONINVENTORY_API UNeonItemFragment_UI : public UNeonItemFragment
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data")
	UTexture2D* Icon;
};
