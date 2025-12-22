#pragma once

#include "CoreMinimal.h"
#include "NeonGridEnums.generated.h"

UENUM(BlueprintType)
enum class ETeamName : uint8
{
	Player UMETA(DisplayName = "Player"),
	Enemies UMETA(DisplayName = "Enemies"),
	Neutrals UMETA(DisplayName = "Neutrals")
};
