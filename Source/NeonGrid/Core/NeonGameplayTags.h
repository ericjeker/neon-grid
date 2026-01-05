// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

/**
 * Native Gameplay Tags defined via macros.
 * These are automatically registered during engine startup.
 */
namespace NeonGameplayTags
{
	// Declare the tags as external variables in this namespace
	NEONGRID_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Invulnerable);
	NEONGRID_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Dead);
	NEONGRID_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Action_Fire);
}