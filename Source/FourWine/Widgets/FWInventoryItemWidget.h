// Troy Records Jr. 2021

#pragma once

#include "CoreMinimal.h"

#include "FWInventoryWidget.h"
#include "Blueprint/UserWidget.h"
#include "FourWine/DataTypes/GameStructs.h"
#include "FWInventoryItemWidget.generated.h"

/**
* 
*/
UCLASS()
class FOURWINE_API UFWInventoryItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void EquipWeapon();

	void Equip();
	void Unequip();

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	UFWInventoryWidget* Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FInventoryItem Item;
	int32 Idx;
	
};
