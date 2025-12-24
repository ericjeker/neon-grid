// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NeonGrid/Components/PatrolPointsComponent.h"
#include "PatrolRoute.generated.h"

UCLASS()
class NEONGRID_API APatrolRoute : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APatrolRoute();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UPatrolPointsComponent* PatrolPointsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBillboardComponent* SpriteComponent; // For editor visibility

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
