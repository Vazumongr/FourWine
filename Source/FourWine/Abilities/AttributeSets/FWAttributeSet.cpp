// Troy Records Jr. 2021


#include "FWAttributeSet.h"

UFWAttributeSet::UFWAttributeSet()
{
	
}

void UFWAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	
}

void UFWAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
}

void UFWAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
}

void UFWAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute,
	const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
}

void UFWAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
}

void UFWAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
}

void UFWAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
}

void UFWAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
}

void UFWAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower)
{
}
