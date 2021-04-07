// Troy Records Jr. 2021


#include "FWAttributeSet.h"


#include "FourWine/FourWine.h"
#include "Net/UnrealNetwork.h"

UFWAttributeSet::UFWAttributeSet()
{
	
}

void UFWAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// This is called whenever attributes change, so for max health/mana we want to scale the current totals to match
    Super::PreAttributeChange(Attribute, NewValue);

	
	UE_LOG(LogAttribute, Warning, TEXT("%s"), *FString(__FUNCTION__));

    // If a Max value changes, adjust current to keep Current % of Current to Max
    if (Attribute == GetMaxHealthAttribute()) // GetMaxHealthAttribute comes from the Macros defined at the top of the header
    {
    	AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
    }
    else if (Attribute == GetMaxStaminaAttribute())
    {
    	AdjustAttributeForMaxChange(Stamina, MaxStamina, NewValue, GetStaminaAttribute());
    }
}

void UFWAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UFWAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UFWAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFWAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFWAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFWAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFWAttributeSet, AttackPower, COND_None, REPNOTIFY_Always);
}

void UFWAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute,
	const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
}

void UFWAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	UE_LOG(LogAttribute, Warning, TEXT("%s"), *FString(__FUNCTION__));
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFWAttributeSet, Health, OldHealth);
}

void UFWAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	UE_LOG(LogAttribute, Warning, TEXT("%s"), *FString(__FUNCTION__));
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFWAttributeSet, MaxHealth, OldMaxHealth);
}

void UFWAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	UE_LOG(LogAttribute, Warning, TEXT("%s"), *FString(__FUNCTION__));
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFWAttributeSet, Stamina, OldStamina);
}

void UFWAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	UE_LOG(LogAttribute, Warning, TEXT("%s"), *FString(__FUNCTION__));
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFWAttributeSet, MaxStamina, OldMaxStamina);
}

void UFWAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower)
{
	UE_LOG(LogAttribute, Warning, TEXT("%s"), *FString(__FUNCTION__));
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFWAttributeSet, AttackPower, OldAttackPower);
}
