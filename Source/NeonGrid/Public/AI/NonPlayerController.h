// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
