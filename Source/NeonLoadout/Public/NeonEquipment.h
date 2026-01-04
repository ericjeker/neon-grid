// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NeonEquipment.generated.h"


UCLASS(BlueprintType, Blueprintable)
class NEONLOADOUT_API ANeonEquipment : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANeonEquipment();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<class APawn> PawnOwner;
	
	UFUNCTION(BlueprintCallable, Category = "Loadout")
	void Equip();
	
	UFUNCTION(BlueprintCallable, Category = "Loadout")
	void Unequip();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Loadout")
	void StartPrimaryAction();
	virtual void StartPrimaryAction_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Loadout")
	void StopPrimaryAction();
	virtual void StopPrimaryAction_Implementation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* EquipmentMesh;
	
};
