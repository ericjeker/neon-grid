// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NPCArchetypeData.h"
#include "Perception/AIPerceptionTypes.h"
#include "Runtime/AIModule/Classes/AIController.h"

#include "NonPlayerController.generated.h"

UCLASS()
class NEONGRID_API ANonPlayerController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANonPlayerController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	
	/** Preception, Sense Configuration, Detection */
	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<class UAISenseConfig_Sight> SenseSightConfig;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	
	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
