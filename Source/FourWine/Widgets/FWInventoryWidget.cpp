// Troy Records Jr. 2021


#include "FWInventoryWidget.h"
#include "FourWine/FourWine.h"

void UFWInventoryWidget::SetInventoryReference(TArray<FInventoryItem> InInventoryItems)
{
	InventoryItems = InInventoryItems;
	UE_LOG(LogInventory, Warning, TEXT("Number of items received: %i"), InventoryItems.Num());
}
