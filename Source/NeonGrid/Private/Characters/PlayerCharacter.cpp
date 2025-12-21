// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "Characters/PlayerCharacter.h"

#include "Projectiles/BaseProjectile.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Camera Boom (spring arm)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1600.0f;
	CameraBoom->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));
	// Camera is not zooming in when it hits an object
	CameraBoom->bDoCollisionTest = false;
	// Fixed angle, doesn't follow character rotation
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;

	// Create Follow Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Configure character movement for top-down shooter
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	// Character moves towards the direction of input
	CharacterMovementComponent->bOrientRotationToMovement = false;
	CharacterMovementComponent->RotationRate = FRotator(0.0f, 640.0f, 0.0f);
	CharacterMovementComponent->bConstrainToPlane = true;
	CharacterMovementComponent->bSnapToPlaneAtStart = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called every frame
void APlayerCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
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

		// This draws a straight line from the camera to the 3D world
		if (FHitResult TraceHitResult; PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, TraceHitResult))
		{
			const FVector TargetLocation = TraceHitResult.ImpactPoint;
			FVector LookDir = TargetLocation - GetTargetLocation();

			// We don't need the Z axis
			LookDir.Z = 0.0f;

			if (!LookDir.IsNearlyZero())
			{
				SetActorRotation(LookDir.Rotation());
			}
		}
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		// TODO: Replace this with AbilitySystemComponent.TryActivateAbilityByTag
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Fire);
	}
}

void APlayerCharacter::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	if (GetController() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller not found"));
		return;
	}

	// Find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// Get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// Get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// Add movement 
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void APlayerCharacter::Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (LookAxisVector.IsNearlyZero())
	{
		return;
	}

	const FRotator LookAtRotation = FVector(LookAxisVector.Y, LookAxisVector.X, 0.0f).Rotation();
	SetActorRotation(LookAtRotation);
}

// TODO: move this to a FireProjectile ability
void APlayerCharacter::Fire()
{
	if (ProjectileClass)
	{
		const float CurrentTime = GetWorld()->GetTimeSeconds();
		if (CurrentTime - LastFireTime < FireRate)
		{
			return;
		}

		LastFireTime = CurrentTime;

		const FVector SpawnLocation = GetActorLocation() + (GetActorForwardVector() * 100.0f);
		const FRotator SpawnRotation = GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		// SPAWN!
		GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	}
}
