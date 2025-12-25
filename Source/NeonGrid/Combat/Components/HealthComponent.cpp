// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "HealthComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NeonGrid/Combat/Attributes/CoreAttributeSet.h"
#include "NeonGrid/Core/NeonCharacter.h"


UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UHealthComponent::IsDead() const
{
	const AActor* Owner = GetOwner();
	if (!Owner)
	{
		return false;
	}

	// Get the AbilitySystemComponent (AttributeSet is owned by ASC, not the actor)
	const IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(Owner);
	if (!ASCInterface)
	{
		return false;
	}

	const UAbilitySystemComponent* ASC = ASCInterface->GetAbilitySystemComponent();
	if (!ASC)
	{
		return false;
	}

	// Get the AttributeSet from the ASC
	const UCoreAttributeSet* AttributeSet = ASC->GetSet<UCoreAttributeSet>();
	if (!AttributeSet)
	{
		return false;
	}

	return AttributeSet->GetHealth() <= 0.0f;
}

void UHealthComponent::OnDeath_Implementation()
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	// Try to cast to Character for character-specific death logic
	if (ACharacter* CharacterOwner = Cast<ACharacter>(Owner))
	{
		// Disable collision
		if (UCapsuleComponent* CapsuleComp = CharacterOwner->GetCapsuleComponent())
		{
			CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		// Disable movement
		if (UCharacterMovementComponent* MovementComp = CharacterOwner->GetCharacterMovement())
		{
			MovementComp->DisableMovement();
			MovementComp->StopMovementImmediately();
		}

		// Detach from controller
		CharacterOwner->DetachFromControllerPendingDestroy();
	}
	else
	{
		// Generic actor death logic (for non-characters like turrets, destructibles)
		Owner->SetActorEnableCollision(false);
	}

	// Notify external observers
	OnActorDied.Broadcast(Owner);
}
