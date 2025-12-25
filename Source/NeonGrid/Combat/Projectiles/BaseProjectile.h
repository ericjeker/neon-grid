// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "BaseProjectile.generated.h"

UCLASS()
class /**
 * Called when the projectile collides with another object.
 *
 * @param HitComponent The component on this actor that registered the hit.
 * @param OtherActor The other actor involved in the collision, if any.
 * @param OtherComp The specific component on the other actor that was hit, if any.
 * @param NormalImpulse The impulse applied to resolve the collision.
 * @param Hit Detailed information about the hit (impact point, normal, etc.).
 */
NEONGRID_API ABaseProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Simple collision sphere
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent* SphereComp;

	// This component handles the "flying" logic automatically
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UProjectileMovementComponent* ProjectileMovement;

	// Visual of the bullet
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* MeshComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<class UGameplayEffect> DamageEffectClass;

	UFUNCTION(BlueprintNativeEvent, Category = "Projectile")
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};