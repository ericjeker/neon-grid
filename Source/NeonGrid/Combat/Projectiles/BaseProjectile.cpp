// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "BaseProjectile.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ABaseProjectile::ABaseProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 1. Setup Collision
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComp;
	// Set the collision profile as Projectile and enable collision notifications
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->SetNotifyRigidBodyCollision(true);
	SphereComp->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit);

	// 2. Setup Mesh (Visual)
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComp->SetupAttachment(RootComponent);

	// 3. Setup Movement (The magic component)
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f; // No gravity for bullets!

	// Bullets shouldn't live forever
	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
}

/**
 * Handle a collision by applying the configured gameplay damage effect to the hit actor if it supports the Gameplay Ability System.
 *
 * If the collided actor implements IAbilitySystemInterface and has a valid AbilitySystemComponent, this method creates an effect context (with the projectile's instigator), constructs an outgoing gameplay effect spec from DamageEffectClass, and applies that spec to the target's AbilitySystemComponent.
 *
 * @param HitComponent The component on this projectile involved in the collision.
 * @param OtherActor The actor that was hit; must implement IAbilitySystemInterface with a valid AbilitySystemComponent for an effect to be applied.
 * @param OtherComp The component on the other actor involved in the collision.
 * @param NormalImpulse The collision impulse vector.
 * @param Hit Detailed hit information (impact location, normal, etc.).
 */
void ABaseProjectile::OnHit_Implementation(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == GetOwner() || !DamageEffectClass)
	{
		return;
	}

	// Check if the OtherActor is speaking GAS
	const IAbilitySystemInterface* AbilityInterface = Cast<IAbilitySystemInterface>(OtherActor);
	if (!AbilityInterface)
	{
		return;
	}

	UAbilitySystemComponent* TargetASC = AbilityInterface->GetAbilitySystemComponent();
	if (!TargetASC)
	{
		return;
	}

	// Create the effect context, add the instigator and outgoing specs
	FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
	EffectContext.AddInstigator(GetInstigator(), this);

	// Depending on the projectile type, the damage effect class can be different
	const FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, EffectContext);
	if (SpecHandle.IsValid())
	{
		TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}