// Copyright 2021 Troy Records Jr.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FourWine/FourWine.h"
#include "FWGameplayAbility.generated.h"

/**
* 
*/
UCLASS()
class FOURWINE_API UFWGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
	public:
	UFWGameplayAbility();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Ability")
	TArray<TSubclassOf<UGameplayEffect>> GameplayEffects;

	// Abilities with this set will automatically activate when the input is pressed
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EFWAbilityInputID AbilityInputID = EFWAbilityInputID::Ability1;

	// Value to associate an ability with an slot without tying it to an automatically activated input.
	// Passive abilities won't be tied to an input so we need a way to generically associate abilities with slots.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EFWAbilityInputID AbilityID = EFWAbilityInputID::Ability1;

	// Tells an ability to activate immediately when its granted. Used for passive abilities and abilities forced on others.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	bool ActivateAbilityOnGranted = false;

	// If an ability is marked as 'ActivateAbilityOnGranted', activate them immediately when given here
	// Epic's comment: Projects may want to initiate passives or do other "BeginPlay" type of logic here.
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};