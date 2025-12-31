// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NeonItemFragment.h"
#include "NeonItemFragment_Inventory.generated.h"

/**
 * 
 */
UCLASS()
class NEONINVENTORY_API UNeonItemFragment_Inventory : public UNeonItemFragment
{
	GENERATED_BODY()
	
public:
	// Mass in Kilograms (Metric System)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data", meta = (Units = "kg"))
	float MassKg = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data")
	int32 MaxStackSize = 1;

};
