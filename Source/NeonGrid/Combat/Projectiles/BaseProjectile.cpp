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
 * Handles the event triggered when the projectile collides with another object.
 *
 * This method is responsible for managing the effects caused by the projectile's impact,
 * including applying gameplay damage effects to the target actor and destroying the projectile
 * after performing its intended functionality.
 *
 * @param HitComponent The specific component of this actor that was involved in the collision (Collision Sphere).
 * @param OtherActor The actor that the projectile collided with.
 * @param OtherComp The specific component of the other actor involved in the collision (Capsule or Sphere Component).
 * @param NormalImpulse The force of the collision expressed as an impulse vector.
 * @param Hit Detailed information about the hit event, including impact location and normal.
 */
void ABaseProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                            FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == GetOwner() || !DamageEffectClass)
	{
		Destroy();
		return;
	}

	// Check if the OtherActor is speaking GAS
	IAbilitySystemInterface* AbilityInterface = Cast<IAbilitySystemInterface>(OtherActor);
	if (!AbilityInterface)
	{
		Destroy();
		return;
	}

	UAbilitySystemComponent* TargetASC = AbilityInterface->GetAbilitySystemComponent();
	if (!TargetASC)
	{
		Destroy();
		return;
	}

	// Create the effect context, add the instigator and outgoing specs
	FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
	EffectContext.AddInstigator(GetInstigator(), this);

	// Depending on the projectile type, the damage effect class can be different
	FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, EffectContext);
	if (SpecHandle.IsValid())
	{
		TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}

	Destroy();
}

// Called every frame
void ABaseProjectile::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}
