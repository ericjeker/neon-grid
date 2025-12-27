// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * Singleton containing native Gameplay Tags
 */
struct FNeonGameplayTags
{
public:
	static const FNeonGameplayTags& Get() { return GameplayTags; }

	static void InitializeNativeTags();

	// Tag for God Mode / Invulnerability
	FGameplayTag State_Invulnerable;

protected:
private:
	static FNeonGameplayTags GameplayTags;
};