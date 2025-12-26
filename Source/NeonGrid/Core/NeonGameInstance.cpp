// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "NeonGameInstance.h"
#include "NeonGrid/Core/NeonGameplayTags.h"

void UNeonGameInstance::Init()
{
	Super::Init();

	// Initialize our native gameplay tags
	FNeonGameplayTags::InitializeNativeTags();
}
