// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "NeonEquipmentTypes.generated.h"

UENUM(BlueprintType)
enum class ENeonAttachmentSocket : uint8
{
	None,
	LeftHand,
	RightHand,
};

UENUM(BlueprintType)
enum class ENeonEquipmentSlot : uint8
{
	Primary,
	Secondary,
	Tactical,
	Consumable,
	Accessory
};
