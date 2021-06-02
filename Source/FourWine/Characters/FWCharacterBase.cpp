// Copyright 2021 Troy Records Jr.

#include "FWCharacterBase.h"

#include "FourWine/Abilities/FWAbilitySystemComponent.h"
#include "FourWine/Abilities/AttributeSets/FWAttributeSet.h"
#include "FourWine/Abilities/FWGameplayAbility.h"
#include "GameFramework/PlayerState.h"

AFWCharacterBase::AFWCharacterBase(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	bAlwaysRelevant = true;
}

UAbilitySystemComponent* AFWCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

bool AFWCharacterBase::IsAlive() const
{
	return GetHealth() > 0.0f;
}

float AFWCharacterBase::GetHealth() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetHealth();
	}

	return 0.0f;
}

float AFWCharacterBase::GetMaxHealth() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxHealth();
	}

	return 0.0f;
}

float AFWCharacterBase::GetStamina() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetStamina();
	}

	return 0.0f;
}

float AFWCharacterBase::GetMaxStamina() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxStamina();
	}

	return 0.0f;
}

float AFWCharacterBase::GetAttackPower() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetAttackPower();
	}

	return 0.0f;
}

void AFWCharacterBase::Die()
{
	// Only runs on Server
	RemoveCharacterAbilities();

	OnCharacterDied.Broadcast(this);

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->CancelAllAbilities();
	}

	UE_LOG(LogPlayer, Warning, TEXT("This player is die: %s"), *GetName());

	Destroy();
	
}

void AFWCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AFWCharacterBase::AddCharacterAbilities(TArray<TSubclassOf<UFWGameplayAbility>>& AbilitiesToAdd)
{
	if(GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid())
	{
		return;
	}
	UE_LOG(LogWeapon, Warning, TEXT("Ability list is size %i"), AbilitiesToAdd.Num());
	for(TSubclassOf<UFWGameplayAbility>& Ability : AbilitiesToAdd)
	{
		AbilitySystemComponent->GiveAbility(
            FGameplayAbilitySpec(Ability, 1, static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID), this));
		bAbilitiesGranted = true;
	}
}

void AFWCharacterBase::InitializeAttributes()
{
	if(!AbilitySystemComponent.IsValid())
	{
		UE_LOG(LogAttribute, Warning, TEXT("ASC is not valid"));
		return;
	}
	if(!DefaultAttributes)
	{
		UE_LOG(LogAttribute, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, 1, EffectContextHandle);
	if(NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}

void AFWCharacterBase::RemoveCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || !AbilitySystemComponent->CharacterAbilitiesGiven)
	{
		return;
	}

	// Remove any abilities added from a previous call. This checks to make sure the ability is in the startup 'CharacterAbilities' array.
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if ((Spec.SourceObject == this) && DefaultAbilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	// Do in two passes so the removal happens after we have the full list
	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = false;
}
