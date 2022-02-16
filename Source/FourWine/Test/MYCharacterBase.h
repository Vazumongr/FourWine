// Troy Records Jr. 2021

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"

#include "MYCharacterBase.generated.h"

/**
 * This is the base class for all characters implementing an ability system
 */
UCLASS()
class FOURWINE_API AMYCharacterBase : public ACharacter
{
    GENERATED_BODY()

public:
	AMYCharacterBase();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MYCharacterBase|Combat")
	TSubclassOf<class AActor> LeftHandWeaponClass;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="MYCharacterBase|Combat")
	class AActor* LeftHandWeapon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MYCharacterBase|Combat")
	TSubclassOf<class AActor> RightHandWeaponClass;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="MYCharacterBase|Combat")
	class AActor* RightHandWeapon;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="MYCharacterBase|Combat")
	class AActor* WeaponItemThing;
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category="MYCharacterBase|Combat")
	TSubclassOf<class AActor> WeaponClass;
};