// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "FireProjectileAbility.h"

#include "GameFramework/Character.h"
#include "NeonGrid/Combat/Projectiles/BaseProjectile.h"

UFireProjectileAbility::UFireProjectileAbility()
{
	// Default settings (override from parent)
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	// Tags can be used to block other actions while firing, or cooldowns
	// AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Fire")));
}

void UFireProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	// Commit Ability (Check costs, cooldowns, etc.)
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Logic: Spawning Actor (Server Only)
	// Spawning usually happens on Authority to ensure replication.
	SpawnProjectile();

	// End the ability
	// Since this is an instant fire action, we end it immediately. 
	// If you had a casting animation, you would wait for the montage to finish.
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UFireProjectileAbility::SpawnProjectile_Implementation()
{
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	const FGameplayAbilityActivationInfo ActivationInfo = GetCurrentActivationInfo();
	
	// TODO: Use WeaponData in the Pawn to know which ProjectileClass to spawn
	if (HasAuthority(&ActivationInfo) && Character && ProjectileClass && GetWorld())
	{
		const FVector SpawnLocation = Character->GetActorLocation() + (Character->GetActorForwardVector() * 100.0f);
		const FRotator SpawnRotation = Character->GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Character;
		SpawnParams.Instigator = Character;

		GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	}
}
