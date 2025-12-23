// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "NeonGridEnums.h"
#include "Abilities/GameplayAbility.h"

#include "NeonCharacter.generated.h"

UCLASS()
class NEONGRID_API ANeonCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANeonCharacter();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// IAbilitySystemInterface implementation
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	/** Grants startup abilities defined in DefaultAbilities */
	void GiveDefaultAbilities();

	/** Ability System */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	EGameplayEffectReplicationMode AscReplicationMode = EGameplayEffectReplicationMode::Minimal;

	/** 
	 * Map of Input IDs to Ability Classes.
	 * Allows configuring which ability triggers on "Fire", "Ability1", etc.
	 */
	UPROPERTY(EditAnywhere, Category = "GAS")
	TMap<EAbilityInputID, TSubclassOf<UGameplayAbility>> DefaultAbilities;

	/**
	 * Represents a set of core attributes for the character, such as health, stamina, or other custom stats.
	 * Used in conjunction with the Ability System to manage attribute-based gameplay mechanics.
	 */
	UPROPERTY()
	class UCoreAttributeSet* AttributeSet;

private:
	// Tracks granted abilities to remove them if needed
	TArray<FGameplayAbilitySpecHandle> DefaultAbilityHandles;
};
