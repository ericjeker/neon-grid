// Copyright Eric Jeker, Inc. All Rights Reserved.

#include "NeonGameplayTags.h"

namespace NeonGameplayTags
{
	// Define the tags. The 2nd argument is the actual tag string in the editor.
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Invulnerable, "State.Invulnerable", "Status tag indicating an actor is God-like and takes no damage");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Dead, "State.Dead", "Status tag indicating an actor is dead");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Action_FireEquipment, "Input.Action.FireEquipment", "Input tag for Fire");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Action_CycleEquipment, "Input.Action.CycleEquipment", "Input tag for Cycle");
}