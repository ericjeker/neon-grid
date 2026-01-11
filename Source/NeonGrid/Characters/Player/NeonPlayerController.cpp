// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "NeonPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NeonInteractionComponent.h"
#include "NeonPlayerCharacter.h"
#include "Blueprint/UserWidget.h"

ANeonPlayerController::ANeonPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ANeonPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Only create the HUD on the local machine (not the server)
	if (IsLocalController() && HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();
		}
	}

	// Setup Input Mode
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer()))
	{
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

/**
 * InputAction can either be native or ability-based. Native actions bypass the GAS pipeline, don't check for tags
 * or cost or cooldowns. The ability actions go through the GAS pipeline using gameplay tags.
 */
void ANeonPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Bind Movement (native)
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this,
			                                   &ANeonPlayerController::Move);
		}

		// Bind Look (native)
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this,
			                                   &ANeonPlayerController::Look);
		}

		// Bind Fire (ability)
		if (FireAction)
		{
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this,
			                                   &ANeonPlayerController::AbilityPressed, EAbilityInputID::Fire);
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this,
			                                   &ANeonPlayerController::AbilityReleased, EAbilityInputID::Fire);
		}

		// Bind Cycle Equipment Slot (event-based)
		if (CycleEquipmentAction)
		{
			EnhancedInputComponent->BindAction(CycleEquipmentAction, ETriggerEvent::Triggered, this,
			                                   &ANeonPlayerController::CycleEquipment);
		}

		// Bind Interact
		if (InteractAction)
		{
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this,
			                                   &ANeonPlayerController::AbilityPressed, EAbilityInputID::Interact);
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this,
			                                   &ANeonPlayerController::AbilityReleased, EAbilityInputID::Interact);
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
				const FRotator TargetRotation = LookDir.Rotation();
				const FRotator CurrentRotation = ControlledChar->GetActorRotation();

				const FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime,
				                                              MouseRotationSpeed);
				ControlledChar->SetActorRotation(NewRotation);
			}
		}
	}
}

void ANeonPlayerController::Move(const FInputActionValue& Value)
{
	if (ANeonPlayerCharacter* ControlledChar = Cast<ANeonPlayerCharacter>(GetPawn()))
	{
		ControlledChar->Move(Value.Get<FVector2D>());
	}
}

/** Called only when using a gamepad. */
void ANeonPlayerController::Look(const FInputActionValue& Value)
{
	if (ANeonPlayerCharacter* ControlledChar = Cast<ANeonPlayerCharacter>(GetPawn()))
	{
		ControlledChar->Look(Value.Get<FVector2D>());
	}
}

void ANeonPlayerController::CycleEquipment(const FInputActionValue& Value)
{
	const float AxisValue = Value.Get<float>();
	
	// Filter out nearly zero noise
	if (FMath::IsNearlyZero(AxisValue))
	{
		return;
	}

	if (ANeonPlayerCharacter* ControlledChar = Cast<ANeonPlayerCharacter>(GetPawn()))
	{
		// Create the gameplay event payload
		FGameplayEventData Payload;
		// This will tell if we cycle the next or previous equipment
		Payload.EventMagnitude = (AxisValue > 0) ? 1.0f : -1.0f;
		Payload.Instigator = this;
		Payload.Target = ControlledChar;

		// Send the event.
		// Note: You must define this tag in your project (e.g. DefaultGameplayTags.ini or via Asset Manager)
		const FGameplayTag CycleTag = FGameplayTag::RequestGameplayTag(FName("Input.Action.CycleEquipment"));

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(ControlledChar, CycleTag, Payload);
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
