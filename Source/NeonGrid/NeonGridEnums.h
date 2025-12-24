#pragma once

#include "CoreMinimal.h"
#include "NeonGridEnums.generated.h"

/**
 * @enum ETeamName
 * @brief Enumerates the team affiliations within the game.
 *
 * This enumeration defines the possible team affiliations that entities
 * in the game can be assigned to. Each enumerator is associated with a display name
 * for use in the editor and gameplay logic.
 */
UENUM(BlueprintType)
enum class ETeamName : uint8
{
	Player UMETA(DisplayName = "Player"),
	Enemies UMETA(DisplayName = "Enemies"),
	Neutrals UMETA(DisplayName = "Neutrals")
};

/**
 * @enum EAbilityInputID
 * @brief Enumerates input actions for abilities used within the game.
 *
 * This enumeration represents the various input IDs that can be mapped to player actions or abilities.
 * Each enumerator is associated with a human-readable display name for ease of use in the editor.
 */
UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	None UMETA(DisplayName = "None"),
	Confirm UMETA(DisplayName = "Confirm"),
	Cancel UMETA(DisplayName = "Cancel"),
	Fire UMETA(DisplayName = "Fire"),
	Ability1 UMETA(DisplayName = "Ability1"),
	Ability2 UMETA(DisplayName = "Ability2"),
	Sprint UMETA(DisplayName = "Sprint"),
	Crouch UMETA(DisplayName = "Crouch")
};

namespace NeonGridAIKeys
{
	const FName TargetActor = TEXT("TargetActor");
	const FName OriginLocation = TEXT("OriginLocation");
	const FName ShouldPatrolFromOrigin = TEXT("ShouldPatrolFromOrigin");
	const FName PatrolLocation = TEXT("PatrolLocation");
	const FName PatrolPointIndex = TEXT("PatrolPointIndex");
	const FName IsReversingPatrol = TEXT("IsReversingPatrol");
}

UENUM(BlueprintType)
enum class EPatrolMode : uint8
{
	Sequential UMETA(DisplayName = "Sequential"),
	PingPong UMETA(DisplayName = "Ping Pong"),
	Random UMETA(DisplayName = "Random")
};
