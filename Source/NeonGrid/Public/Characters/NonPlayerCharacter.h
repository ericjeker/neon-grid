// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"

#include "BaseCharacter.h"
#include "NeonGridEnums.h"

#include "NonPlayerCharacter.generated.h"

/**
 * ANonPlayerCharacter is an AI-controlled character. 
 * This class defines default behavior and properties for non-player characters (NPCs) in the game
 * 
 * @implements IGenericTeamAgentInterface
 */
UCLASS()
class NEONGRID_API ANonPlayerCharacter : public ABaseCharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ANonPlayerCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** IGenericTeamAgentInterface implementation */
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override
	{
		TeamName = static_cast<ETeamName>(TeamID.GetId());
	}

	virtual FGenericTeamId GetGenericTeamId() const override
	{
		return FGenericTeamId(static_cast<uint8>(TeamName));
	}

	/** Returns whether this NPC should patrol from its origin point */
	FORCEINLINE bool ShouldPatrolFromOrigin() const { return bShouldPatrolFromOrigin; };
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	bool bShouldPatrolFromOrigin = true;

	UPROPERTY(EditAnywhere, Category = "AI")
	ETeamName TeamName = ETeamName::Enemies;
};
