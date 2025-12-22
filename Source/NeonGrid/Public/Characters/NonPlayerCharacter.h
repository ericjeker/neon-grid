// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"

#include "NonPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class NEONGRID_API ANonPlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ANonPlayerCharacter();

protected:
	virtual void BeginPlay() override;
	
public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	bool bShouldPatrolFromOrigin = true;
};
