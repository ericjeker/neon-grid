// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "NeonGrid/Core/NeonGameplayTags.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Camera Boom (spring arm)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1600.0f;
	CameraBoom->SetRelativeRotation(FRotator(-60.0f, -45.0f, 0.0f));
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

	// Configure character movement for a top-down shooter
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Character moves towards the direction of input
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->bOrientRotationToMovement = false;
	CharacterMovementComponent->RotationRate = FRotator(0.0f, 640.0f, 0.0f);
	CharacterMovementComponent->bConstrainToPlane = true;
	CharacterMovementComponent->bSnapToPlaneAtStart = true;
	
	// Make character discoverable by AI
	UAIPerceptionStimuliSourceComponent* StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));
	StimuliSource->RegisterForSense(UAISense_Sight::StaticClass());
	StimuliSource->RegisterWithPerceptionSystem();
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Make the player invulnerable (for testing purpose)
	AbilitySystemComponent->AddLooseGameplayTag(FNeonGameplayTags::Get().State_Invulnerable);
}

void APlayerCharacter::Move(const FVector2D& MovementVector)
{
	if (FollowCamera == nullptr) return;
	
	const FRotator CameraRotation = FollowCamera->GetComponentRotation();
	const FRotator YawRotation(0, CameraRotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void APlayerCharacter::Look(const FVector2D& LookVector)
{
	if (LookVector.IsNearlyZero()) return;

	// This logic handles Gamepad look. Mouse look is handled by the Controller Tick trace.
	const FRotator LookAtRotation = FVector(LookVector.Y, LookVector.X, 0.0f).Rotation();
	SetActorRotation(LookAtRotation);
}
