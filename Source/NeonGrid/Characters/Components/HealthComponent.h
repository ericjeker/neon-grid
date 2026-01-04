// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "HealthComponent.generated.h"

// Multicast delegate for external systems
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorDeathSignature, AActor*, DeadActor);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NEONGRID_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

	/** Returns true if the actor is dead */
	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsDead() const;

	/** Called when the actor dies */
	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnDeath();

	/** Broadcast when this actor dies - for external observers */
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnActorDeathSignature OnActorDied;

protected:
	virtual void BeginPlay() override;
};
