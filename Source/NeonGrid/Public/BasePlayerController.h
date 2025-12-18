#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

UCLASS()
class NEONGRID_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ABasePlayerController();
	
protected:
	virtual void BeginPlay() override;
};