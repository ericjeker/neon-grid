// Copyright Eric Jeker, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CoreAttributeSet.generated.h"

// Macro to automatically generate boilerplate for attributes (Getter, Setter, Init)
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class NEONGRID_API UCoreAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UCoreAttributeSet();

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCoreAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCoreAttributeSet, MaxHealth);

	// Meta-attribute for damage handling
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UCoreAttributeSet, Damage);
	
	// This handles logic like clamping Health between 0 and MaxHealth when it changes
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// This handles logic after a change has occurred (e.g., triggering a death event)
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

protected:
	/** Checks whether the owning actor of this attribute set should be considered dead. */
	void CheckOwnerForDeath() const;
};
