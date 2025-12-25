// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "FireProjectileAbility.h"

#include "GameFramework/Character.h"
#include "NeonGrid/Combat/Projectiles/BaseProjectile.h"

/**
 * @brief Constructs the ability with gameplay defaults for instancing and networking.
 *
 * Initializes the ability to use per-actor instancing and a local-predicted net execution policy.
 * Leaves an example (commented) of adding an ability tag for blocking or cooldown behavior.
 */
UFireProjectileAbility::UFireProjectileAbility()
{
	// Default settings (override from parent)
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	// Tags can be used to block other actions while firing, or cooldowns
	// AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Fire")));
}

/**
 * Activates the ability: commits costs/cooldowns, spawns the projectile on the server, and ends the ability.
 *
 * Attempts to commit the ability; if the commit fails the ability is canceled and ended. If the commit
 * succeeds, a projectile is spawned (performed server-authoritatively) and the ability is finished immediately.
 *
 * @param Handle The gameplay ability specification handle for this activation.
 * @param ActorInfo Pointer to contextual information about the actor owning the ability.
 * @param ActivationInfo Metadata about this activation (activation prediction/replication state).
 * @param TriggerEventData Optional event data that triggered this activation (may be nullptr).
 */
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

/**
 * @brief Spawns the configured projectile for the owning character when running on authority.
 *
 * @details If the ability's avatar is a valid character, the projectile class is set, and execution is authoritative,
 * a projectile of `ProjectileClass` is spawned at the character's location offset 100 units forward using the character's rotation.
 * The spawned actor's Owner and Instigator are set to the character. If any prerequisite check fails, no actor is spawned.
 */
void UFireProjectileAbility::SpawnProjectile_Implementation()
{
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	const FGameplayAbilityActivationInfo ActivationInfo = GetCurrentActivationInfo();
	
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