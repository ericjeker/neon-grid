// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NeonItemFragment.generated.h"

class UNeonItemInstance;

/**
 * Base class for all Item Fragments.
 * Contains logic/data that defines how an item behaves in specific contexts.
 */
UCLASS(Abstract, DefaultToInstanced, EditInlineNew, BlueprintType)
class NEONINVENTORY_API UNeonItemFragment : public UObject
{
	GENERATED_BODY()
	
public:
	/**
	 * Virtual function called when the ItemInstance is first created.
	 * Useful for initializing dynamic tags, stats, or attributes based on this fragment.
	 */
	virtual void OnInstanceCreated(UNeonItemInstance* Instance) const { }
};
