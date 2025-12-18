// Fill out your copyright notice in the Description page of Project Settings.


#include "NeonGrid/Public/BaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UCapsuleComponent* CapsulteComp = GetCapsuleComponent();
	if (CapsulteComp)
	{
		CapsulteComp->InitCapsuleSize(42.0f, 96.0f);
		CapsulteComp->SetCollisionProfileName("Character");
		CapsulteComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	// Configure the inherited character mesh
	USkeletalMeshComponent* MeshComp = GetMesh();
	MeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Create Camera Boom (spring arm)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 800.0f;
	CameraBoom->SetRelativeRotation(FRotator(-70.0f, 0.0f, 0.0f));
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
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Move);
	}
}

void ABaseCharacter::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	
	UE_LOG(LogTemp, Warning, TEXT("MovementVector: %f"), MovementVector.X);

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
