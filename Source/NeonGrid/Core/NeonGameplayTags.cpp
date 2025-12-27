// Copyright Eric Jeker, Inc. All Rights Reserved.

#include "NeonGameplayTags.h"
#include "GameplayTagsManager.h"

FNeonGameplayTags FNeonGameplayTags::GameplayTags;

void FNeonGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

	GameplayTags.State_Invulnerable = Manager.AddNativeGameplayTag(
		FName("State.Invulnerable"),
		FString("Status tag indicating an actor is God-like and takes no damage")
	);
}