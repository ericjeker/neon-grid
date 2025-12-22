// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
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

	// Reference a Behavior Tree asset in Blueprint
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName OriginLocationKeyName = TEXT("OriginLocation");
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName ShouldPatrolFromOriginKeyName = TEXT("ShouldPatrolFromOrigin");

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TargetActorKeyName = TEXT("TargetActor");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;

	/** Preception, Sense Configuration, Detection */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<class UAISenseConfig_Sight> SightConfig;
	
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	
	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
