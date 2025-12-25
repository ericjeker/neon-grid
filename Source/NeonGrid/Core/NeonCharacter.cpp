// Copyright Eric Jeker, Inc. All Rights Reserved.

#include "NeonCharacter.h"

#include "AbilitySystemComponent.h"
#include "NeonGrid/Combat/Attributes/CoreAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ANeonCharacter::ANeonCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (UCapsuleComponent* CapsuleComp = GetCapsuleComponent())
	{
		CapsuleComp->InitCapsuleSize(42.0f, 96.0f);
		CapsuleComp->SetCollisionProfileName("Character");
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	// Configure the inherited character mesh
	USkeletalMeshComponent* MeshComp = GetMesh();
	MeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Ability system initialization
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(AscReplicationMode);
	
	// Default attribute set for our character
	AttributeSet = CreateDefaultSubobject<UCoreAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* ANeonCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void ANeonCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ANeonCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Initialize GAS Abilities on Server
	if (HasAuthority() && AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		GiveDefaultAbilities();
	}
}

bool ANeonCharacter::IsDead() const
{
	if (!AttributeSet)
	{
		return false;
	}
	
	return AttributeSet->GetHealth() <= 0.0f;
}

void ANeonCharacter::OnDeath_Implementation()
{
	// Base implementation - can be overridden in child classes
	// Disable collision
	if (UCapsuleComponent* CapsuleComp = GetCapsuleComponent())
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Disable movement
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		MovementComp->DisableMovement();
		MovementComp->StopMovementImmediately();
	}

	// Detach from controller
	DetachFromControllerPendingDestroy();
}

void ANeonCharacter::GiveDefaultAbilities()
{
	if (!HasAuthority() || !AbilitySystemComponent) return;

	for (const auto& AbilityPair : DefaultAbilities)
	{
		EAbilityInputID InputID = AbilityPair.Key;
		TSubclassOf<UGameplayAbility> AbilityClass = AbilityPair.Value;

		if (AbilityClass)
		{
			// Grant the ability and bind it to the Input ID from the enum
			FGameplayAbilitySpec AbilitySpec(AbilityClass, 1, static_cast<int32>(InputID), this);
			DefaultAbilityHandles.Add(AbilitySystemComponent->GiveAbility(AbilitySpec));
		}
	}
}

// Called every frame
void ANeonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ANeonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
