// Copyright 2021 Troy Records Jr.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "FWAbilitySystemComponent.generated.h"

/**
* 
*/
UCLASS()
class FOURWINE_API UFWAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
	public:
	bool CharacterAbilitiesGiven = false;

	virtual void AbilityLocalInputPressed(int32 InputID) override;
};
