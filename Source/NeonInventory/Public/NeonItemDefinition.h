// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Fragments/NeonItemFragment.h"

#include "NeonItemDefinition.generated.h"


/**
 * Static data describing an item type.
 * Inherit from this in Blueprint to create items (Right Click -> Miscellaneous -> Data Asset).
 * 
 * This is a read-only, shared, global data asset that defines the properties and behavior of an item type. Behaviors
 * are defined in fragments attached to this definition.
 */
UCLASS(Blueprintable, BlueprintType)
class NEONINVENTORY_API UNeonItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** The class type used to create item instances associated with this item definition. */
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<UNeonItemInstance> InstanceType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FText DisplayName;

	/** Fragments are Stateless Definitions. They define the "Rules" or "Traits" of an item. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Fragments")
	TArray<TObjectPtr<UNeonItemFragment>> Fragments;

	/**
	 * C++ Helper: Finds a specific fragment from the static ItemDef.
	 * Usage: const UFragment_Stats* Stats = ItemDef->FindFragment<UFragment_Stats>();
	 */
	template <typename T>
	const T* FindFragment() const
	{
		for (const TObjectPtr<UNeonItemFragment>& Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA<T>())
			{
				return Cast<T>(Fragment.Get());
			}
		}

		return nullptr;
	}
		
	/** 
	 * Blueprint Helper: Finds a fragment by class type. 
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (DeterminesOutputType = "FragmentClass"))
	const UNeonItemFragment* FindFragmentByClass(TSubclassOf<UNeonItemFragment> FragmentClass) const;
};
