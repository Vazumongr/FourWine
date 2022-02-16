// Troy Records Jr. 2021

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "FourWine/DataTypes/GameStructs.h"
#include "FWInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class FOURWINE_API UFWInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetupInventoryDisplay();
	void Setup();
	void TearDown();
	void UpdateDisplay(class UFWInventoryItemWidget* NewItem);

	UPROPERTY()
	class UFWInventoryComponent* InventoryComponent;
	UPROPERTY()
	class UFWInventoryItemWidget* EquippedItem;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UFWInventoryItemWidget> ItemClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UScrollBox* ItemBoxes;
	
};
