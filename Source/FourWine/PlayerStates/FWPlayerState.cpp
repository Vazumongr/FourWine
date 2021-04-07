﻿// Copyright 2021 Troy Records Jr.

#include "FWPlayerState.h"

#include "AbilitySystemComponent.h"
#include "FourWine/Abilities/AttributeSets/FWAttributeSet.h"
#include "FourWine/FourWine.h"

AFWPlayerState::AFWPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->ReplicationMode = EGameplayEffectReplicationMode::Full;

	NetUpdateFrequency = 100.f;
}

void AFWPlayerState::PostInitializeComponents()
{
	AttributeSet = NewObject<UFWAttributeSet>(this);

	Super::PostInitializeComponents();
}

UAbilitySystemComponent* AFWPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UFWAttributeSet* AFWPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

float AFWPlayerState::GetHealth() const
{
	return AttributeSet->GetHealth();
}

float AFWPlayerState::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

float AFWPlayerState::GetStamine() const
{
	return AttributeSet->GetStamina();
}

float AFWPlayerState::GetMaxStamina() const
{
	return AttributeSet->GetMaxStamina();
}

float AFWPlayerState::GetAttackPower() const
{
	return AttributeSet->GetAttackPower();
}

void AFWPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if(AbilitySystemComponent)
	{
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AFWPlayerState::HealthChanged);
		MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute()).AddUObject(this, &AFWPlayerState::MaxHealthChanged);
		StaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaAttribute()).AddUObject(this, &AFWPlayerState::StaminaChanged);
		MaxStaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxStaminaAttribute()).AddUObject(this, &AFWPlayerState::MaxStaminaChanged);
		AttackPowerChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetAttackPowerAttribute()).AddUObject(this, &AFWPlayerState::AttackPower);
	}
}

void AFWPlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	float Health = Data.NewValue;
	UE_LOG(LogAttribute, Warning, TEXT("Health: %f"), Health);
	// Update HUD stuff
}

void AFWPlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	float MaxHealth = Data.NewValue;
	UE_LOG(LogAttribute, Warning, TEXT("MaxHealth: %f"), MaxHealth);
}

void AFWPlayerState::StaminaChanged(const FOnAttributeChangeData& Data)
{
	float Stamina = Data.NewValue;
	UE_LOG(LogAttribute, Warning, TEXT("Stamina: %f"), Stamina);
}

void AFWPlayerState::MaxStaminaChanged(const FOnAttributeChangeData& Data)
{
	float MaxStamina = Data.NewValue;
	UE_LOG(LogAttribute, Warning, TEXT("MaxStamina: %f"), MaxStamina);
}

void AFWPlayerState::AttackPower(const FOnAttributeChangeData& Data)
{
	float AttackPower = Data.NewValue;
	UE_LOG(LogAttribute, Warning, TEXT("AttackPower: %f"), AttackPower);
}