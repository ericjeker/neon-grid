// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NeonItemDefinition.h"
#include "NeonItemInstance.generated.h"

class UNeonItemFragment;

UCLASS(BlueprintType, Blueprintable)
class NEONINVENTORY_API UNeonItemInstance : public UObject
{
	GENERATED_BODY()
	
public:
	
	// The static data asset this instance was created from (e.g., DA_Rifle)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<UNeonItemDefinition> ItemDef;
	
	/** 
	 * C++ Helper: Finds a specific fragment from the static ItemDef.
	 * Usage: const UFragment_Stats* Stats = Instance->FindFragment<UFragment_Stats>();
	 */
	template <typename T>
	const T* FindFragment() const
	{
		if (ItemDef)
		{
			return ItemDef->FindFragment<T>();
		}
		
		return nullptr;
	}

	/** 
	 * Blueprint Helper: Finds a fragment by class type. 
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (DeterminesOutputType = "FragmentClass"))
	const UNeonItemFragment* FindFragmentByClass(TSubclassOf<UNeonItemFragment> FragmentClass) const;
};