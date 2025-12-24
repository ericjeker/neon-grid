// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NeonGrid/NeonGridEnums.h"

#include "PatrolPointsComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NEONGRID_API UPatrolPointsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPatrolPointsComponent();

	UFUNCTION(BlueprintCallable, Category = "Patrol")
	FVector GetNextPatrolPoint(int32& CurrentIndex, bool& bIsReversing) const;

	UFUNCTION(BlueprintCallable, Category = "Patrol")
	int32 GetPatrolPointCount() const { return PatrolPoints.Num(); }

	UFUNCTION(BlueprintCallable, Category = "Patrol")
	FVector GetPatrolPointWorldLocation(int32 Index) const;

	UFUNCTION(BlueprintCallable, Category = "Patrol")
	EPatrolMode GetPatrolMode() const { return PatrolMode; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol")
	TArray<FVector> PatrolPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol")
	EPatrolMode PatrolMode = EPatrolMode::Sequential;

	/**
	 * bUseLocalSpace determines whether patrol points are stored relative to the APatrolRoute actor's transform or as 
	 * absolute world positions. This allows the PatrolRoute to be moved around without needing to update each patrol 
	 * point.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol")
	bool bUseLocalSpace = true;
};
