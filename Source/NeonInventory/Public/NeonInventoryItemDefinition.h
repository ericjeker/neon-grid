// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "NeonInventoryItemDefinition.generated.h"

/**
 * Static data describing an item type.
 * Inherit from this in Blueprint to create items (Right Click -> Miscellaneous -> Data Asset).
 */
UCLASS(Blueprintable, BlueprintType)
class NEONINVENTORY_API UNeonInventoryItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data")
	UTexture2D* Icon;

	// Mass in Kilograms (Metric System)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data", meta = (Units = "kg"))
	float MassKg = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data")
	int32 MaxStackSize = 1;
};
