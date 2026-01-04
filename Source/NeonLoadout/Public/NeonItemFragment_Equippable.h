// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Fragments/NeonItemFragment.h"
#include "NeonItemFragment_Equippable.generated.h"


// Forward declarations
class ANeonEquipment;

UCLASS()
class NEONLOADOUT_API UNeonItemFragment_Equippable : public UNeonItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equippable")
	TSubclassOf<ANeonEquipment> EquipmentClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equippable")
	FName AttachmentSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equippable")
	FTransform AttachmentOffset;
};
