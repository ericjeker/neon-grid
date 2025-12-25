// Copyright Eric Jeker, Inc. All Rights Reserved.


#include "CoreAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "NeonGrid/Core/NeonCharacter.h"

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
		// Store the damage amount and clear the meta-attribute
		const float LocalDamageDone = GetDamage();
		SetDamage(0.f);

		if (LocalDamageDone > 0.0f)
		{
			// Apply the health change
			const float NewHealth = GetHealth() - LocalDamageDone;
			SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
			
			// Check for death
			if (GetHealth() <= 0.0f)
			{
				if (AActor* OwnerActor = GetOwningActor())
				{
					if (ANeonCharacter* Character = Cast<ANeonCharacter>(OwnerActor))
					{
						Character->OnDeath();
					}
				}
			}
		}
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Clamp again to be safe after modifiers
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
		
		// Check for death even on direct health modification
		if (GetHealth() <= 0.0f)
		{
			if (AActor* OwnerActor = GetOwningActor())
			{
				if (ANeonCharacter* Character = Cast<ANeonCharacter>(OwnerActor))
				{
					Character->OnDeath();
				}
			}
		}
	}
}
