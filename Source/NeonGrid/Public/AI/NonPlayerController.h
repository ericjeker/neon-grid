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

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName OriginLocationKeyName = TEXT("OriginLocation");
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName ShouldPatrolFromOriginKeyName = TEXT("ShouldPatrolFromOrigin");

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TargetActorKeyName = TEXT("TargetActor");

	/** Reference a Behavior Tree asset in Blueprint */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;

	/** Preception, Sense Configuration, Detection */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<class UAISenseConfig_Sight> SightConfig;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	bool bShouldPatrolFromOrigin = true;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	
	
	
	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
