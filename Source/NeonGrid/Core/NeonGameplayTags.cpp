// Copyright Eric Jeker, Inc. All Rights Reserved.

#include "NeonGameplayTags.h"

namespace NeonGameplayTags
{
	// Define the tags. The 2nd argument is the actual tag string in the editor.
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Invulnerable, "State.Invulnerable", "Status tag indicating an actor is God-like and takes no damage");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Dead, "State.Dead", "Status tag indicating an actor is dead");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Action_Fire, "Input.Action.Fire", "Input tag for Fire");
}