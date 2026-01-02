// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "NeonItemDefinition.h"

const UNeonItemFragment* UNeonItemDefinition::FindFragmentByClass(TSubclassOf<UNeonItemFragment> FragmentClass) const
{
	for (const TObjectPtr<UNeonItemFragment>& Fragment : Fragments)
	{
		if (Fragment && Fragment->IsA(FragmentClass))
		{
			return Fragment.Get();
		}
	}

	return nullptr;
}