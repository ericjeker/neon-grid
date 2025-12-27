// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "NeonPlayerController.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerCharacter.h"

ANeonPlayerController::ANeonPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ANeonPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Setup Input Mode
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);

	// Add Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer()))
	{
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ANeonPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Bind Movement
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this,
			                                   &ANeonPlayerController::Move);
		}

		// Bind Look
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this,
			                                   &ANeonPlayerController::Look);
		}

		// Bind Fire
		if (FireAction)
		{
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this,
			                                   &ANeonPlayerController::AbilityPressed, EAbilityInputID::Fire);
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this,
			                                   &ANeonPlayerController::AbilityReleased, EAbilityInputID::Fire);
		}
	}
}

void ANeonPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Handling Mouse Rotation Logic here.
	// We only do this trace if we aren't using Gamepad look input
	bool bIsUsingGamepad = false;

	// Only use mouse rotation if no gamepad input is detected
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (LookAction)
		{
			const FVector2D LookValue = EnhancedInputComponent->GetBoundActionValue(LookAction).Get<FVector2D>();
			if (LookValue.SizeSquared() > 0.1f)
			{
				bIsUsingGamepad = true;
			}
		}
	}

	// Only use mouse rotation if the gamepad is idle
	if (bIsUsingGamepad)
	{
		return;
	}

	FHitResult TraceHitResult;
	if (GetHitResultUnderCursor(ECC_Visibility, false, TraceHitResult))
	{
		if (APawn* ControlledChar = GetPawn())
		{
			const FVector TargetLocation = TraceHitResult.ImpactPoint;
			FVector LookDir = TargetLocation - ControlledChar->GetActorLocation();
			LookDir.Z = 0.0f;

			if (!LookDir.IsNearlyZero())
			{
				ControlledChar->SetActorRotation(LookDir.Rotation());
			}
		}
	}
}

void ANeonPlayerController::Move(const FInputActionValue& Value)
{
	if (APlayerCharacter* ControlledChar = Cast<APlayerCharacter>(GetPawn()))
	{
		ControlledChar->Move(Value.Get<FVector2D>());
	}
}

/** Called only when using a gamepad. */
void ANeonPlayerController::Look(const FInputActionValue& Value)
{
	if (APlayerCharacter* ControlledChar = Cast<APlayerCharacter>(GetPawn()))
	{
		ControlledChar->Look(Value.Get<FVector2D>());
	}
}

void ANeonPlayerController::AbilityPressed(EAbilityInputID InputId)
{
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(GetPawn()))
	{
		if (UAbilitySystemComponent* ASC = ASCInterface->GetAbilitySystemComponent())
		{
			ASC->AbilityLocalInputPressed(static_cast<int32>(InputId));
		}
	}
}

void ANeonPlayerController::AbilityReleased(EAbilityInputID InputId)
{
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(GetPawn()))
	{
		if (UAbilitySystemComponent* ASC = ASCInterface->GetAbilitySystemComponent())
		{
			ASC->AbilityLocalInputReleased(static_cast<int32>(InputId));
		}
	}
}
