// Copyright 2021 Troy Records Jr.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"

#include "FWCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, class AFWCharacterBase*, Character);

/**
* The base Character class for the game. Everything with an AbilitySystemComponent in this game will inherit from this class.
* This class should not be instantiated and instead subclassed.
*/
UCLASS()
class FOURWINE_API AFWCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AFWCharacterBase(const class FObjectInitializer& ObjectInitializer);

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(BlueprintAssignable)
	FCharacterDiedDelegate OnCharacterDied;

	UFUNCTION(BlueprintCallable)
	virtual bool IsAlive() const;

	virtual void RemoveCharacterAbilities();

	/*
	 * Getters and Setters for Attributes
	 */
	UFUNCTION(BlueprintCallable, Category = "FourWine|FWPlayerState|Attributes")
    float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "FourWine|FWPlayerState|Attributes")
    float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "FourWine|FWPlayerState|Attributes")
    float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "FourWine|FWPlayerState|Attributes")
    float GetMaxStamina() const;

	UFUNCTION(BlueprintCallable, Category = "FourWine|FWPlayerState|Attributes")
    float GetAttackPower() const;

	virtual void Die();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bDebugMelee = true;

protected:
	virtual void BeginPlay() override;

	TWeakObjectPtr<class UFWAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<class UFWAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="FourWine|Abilities")
	TArray<TSubclassOf<class UFWGameplayAbility>> DefaultAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="FourWine|Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;
	
	bool bAbilitiesGranted = false;

	virtual void AddCharacterAbilities(TArray<TSubclassOf<class UFWGameplayAbility>>& AbilitiesToAdd);

	virtual void InitializeAttributes();

};