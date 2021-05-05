// Troy Records Jr. 2021


#include "FWInventoryWidget.h"
#include "Components/ScrollBox.h"
#include "FWInventoryItemWidget.h"


#include "FourWine/FourWine.h"

void UFWInventoryWidget::SetInventoryReference(TArray<FInventoryItem> InInventoryItems)
{
	InventoryItems = InInventoryItems;
	UE_LOG(LogInventory, Warning, TEXT("Number of items received: %i"), InventoryItems.Num());
	int32 Idx = 0;
	for(FInventoryItem& Item: InventoryItems)
	{
		UFWInventoryItemWidget* Widget = CreateWidget<UFWInventoryItemWidget>(this, ItemClass);
		Widget->Item = Item;
		Widget->Idx = Idx;
		ItemBoxes->AddChild(Widget);
		Idx++;
	}
}

void UFWInventoryWidget::Setup()
{
	AddToViewport();
	bIsFocusable = true;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	UWorld* World = GetWorld();
	if(!ensure(World)) return;
    
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if(!ensure(PlayerController)) return;
    
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}

void UFWInventoryWidget::TearDown()
{
	RemoveFromViewport();
	bIsFocusable = false;
	
	FInputModeGameOnly InputModeData;
	InputModeData.SetConsumeCaptureMouseDown(false);

	UWorld* World = GetWorld();
	if(!ensure(World)) return;
    
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if(!ensure(PlayerController)) return;
    
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;
}
