// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"

#include "NeonGrid/Core/NeonCharacter.h"
#include "NeonGrid/NPCs/Behaviors/Patrol/PatrolRoute.h"
#include "NPCArchetypeData.h"
#include "NeonGrid/Combat/WeaponData.h"

#include "NonPlayerCharacter.generated.h"

/**
 * ANonPlayerCharacter is an AI-controlled character. 
 * This class defines default behavior and properties for non-player characters (NPCs) in the game
 * 
 * @implements IGenericTeamAgentInterface
 */
UCLASS()
class NEONGRID_API ANonPlayerCharacter : public ANeonCharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ANonPlayerCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** IGenericTeamAgentInterface implementation */
	virtual void SetGenericTeamId(const FGenericTeamId& InTeamID) override
	{
		TeamID = InTeamID;
	}

	virtual FGenericTeamId GetGenericTeamId() const override
	{
		return TeamID;
	}

	/**
	 * Retrieves the archetype data associated with the non-player character (NPC).
	 *
	 * @return A pointer to the UNPCArchetypeData object that holds the NPC's configuration data.
	 */
	UNPCArchetypeData* GetArchetypeData() const { return ArchetypeData; }

protected:
	virtual void BeginPlay() override;

	/**
	 * ArchetypeData represents a reference to the archetype configuration data for non-player characters (NPCs).
	 * This variable holds a pointer to a UNPCArchetypeData object, which defines key attributes and behaviors
	 * of the NPC, such as team affiliation, AI logic, perception parameters, and other customizable settings.
	 *
	 * This property allows for modular configuration and behavior control of NPCs in the game, enabling
	 * dynamic and reusable archetypes.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	TObjectPtr<UNPCArchetypeData> ArchetypeData;

	/**
	 * TODO: replace by a PatrolRouteComponent
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UPatrolPointsComponent* PatrolPointsComponent;

private:
	FGenericTeamId TeamID;
};
