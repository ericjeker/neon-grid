// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "NeonItemDefinition.h"

#include "Fragments/NeonItemFragment.h"


bool UNeonItemDefinition::ExecutePickupFragments(AActor* Picker, const int32 Quantity) const
{
	for (const UNeonItemFragment* Fragment : Fragments)
	{
		if (Fragment && Fragment->OnItemPickedUp(Picker, const_cast<UNeonItemDefinition*>(this), Quantity))
		{
			// First fragment that handles it wins
			return true;
		}
	}
	
	// No fragment handled it
	return false;
}
