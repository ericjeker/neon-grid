// Fill out your copyright notice in the Description page of Project Settings.


#include "NeonGrid/Public/BasePlayerController.h"

ABasePlayerController::ABasePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
}
