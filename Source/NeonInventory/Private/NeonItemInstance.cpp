// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "NeonItemInstance.h"

const UNeonItemFragment* UNeonItemInstance::FindFragmentByClass(TSubclassOf<UNeonItemFragment> FragmentClass) const
{
	if (!FragmentClass)
	{
		return nullptr;
	}
	
	if (ItemDef)
	{
		return ItemDef->FindFragmentByClass(FragmentClass);
	}
	
	return nullptr;
}
