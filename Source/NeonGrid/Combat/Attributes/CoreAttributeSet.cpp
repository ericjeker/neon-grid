// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "CoreAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "NeonGrid/Core/Characters/NeonCharacter.h"
#include "NeonGrid/Core/NeonGameplayTags.h"

UCoreAttributeSet::UCoreAttributeSet() : Health(100.f), MaxHealth(100.f)
{
}

void UCoreAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
}

void UCoreAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		ExecuteDamageEffect(Data);
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		ExecuteHealthEffect();
	}
}

void UCoreAttributeSet::ExecuteDamageEffect(const struct FGameplayEffectModCallbackData& Data)
{
	if (!GetOwningActor()) return;

	// Check for the Invulnerable tag on the target (the owner of this AttributeSet)
	const FGameplayTag InvulnerableTag = FNeonGameplayTags::Get().State_Invulnerable;
	if (Data.Target.HasMatchingGameplayTag(InvulnerableTag))
	{
		SetDamage(0.f);
		UE_LOG(LogTemp, Warning, TEXT("Damage effect ignored on %s because it is Invulnerable!"),
		       *GetOwningActor()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Damage effect applied on %s!"), *GetOwningActor()->GetName());
	}

	// Store the damage amount and clear the meta-attribute
	const float LocalDamageDone = GetDamage();
	SetDamage(0.f);

	if (LocalDamageDone > 0.0f)
	{
		// Apply the health change
		const float NewHealth = GetHealth() - LocalDamageDone;
		SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));

		// Get the instigator from the effect context
		AActor* DamageInstigator = nullptr;
		if (Data.EffectSpec.GetContext().GetInstigator())
		{
			DamageInstigator = Data.EffectSpec.GetContext().GetInstigator();
		}

		// Broadcast damage event
		if (OnDamageTaken.IsBound())
		{
			OnDamageTaken.Broadcast(DamageInstigator, LocalDamageDone);
		}

		// Check for death
		CheckOwnerForDeath();
	}
}

void UCoreAttributeSet::ExecuteHealthEffect()
{
	// Clamp again to be safe after modifiers
	SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

	// Check for death even on direct health modification
	CheckOwnerForDeath();
}

void UCoreAttributeSet::CheckOwnerForDeath() const
{
	if (GetHealth() <= 0.0f)
	{
		if (const AActor* OwnerActor = GetOwningActor())
		{
			if (const auto HealthComp = OwnerActor->FindComponentByClass<UHealthComponent>())
			{
				HealthComp->OnDeath();
			}
		}
	}
}
