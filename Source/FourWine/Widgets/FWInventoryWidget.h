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

	void SetInventoryReference(TArray<struct FInventoryItem> InInventoryItems);
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ItemName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<struct FInventoryItem> InventoryItems;
	
};
