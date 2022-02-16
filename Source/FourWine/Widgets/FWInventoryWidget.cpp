// Troy Records Jr. 2021


#include "FWInventoryWidget.h"
#include "FWInventoryItemWidget.h"
#include "Components/ScrollBox.h"
#include "FourWine/ActorComponents/FWInventoryComponent.h"
#include "FourWine/Characters/FWPlayerCharacter.h"
#include "FourWine/FourWine.h"

void UFWInventoryWidget::SetupInventoryDisplay()
{
	if(!ensure(InventoryComponent))
	{
		UE_LOG(LogInventory, Error, TEXT("InventoryComponent not found!"));
		return;
	}

	int32 Idx = 0;
	for(const FInventoryItem& Item : InventoryComponent->Inventory)
	{
		UFWInventoryItemWidget* Widget = CreateWidget<UFWInventoryItemWidget>(this, ItemClass);
		Widget->Item = Item;
		Widget->Idx = Idx;
		Widget->Owner = this;
		ItemBoxes->AddChild(Widget);
		Idx++;
	}
	
}

void UFWInventoryWidget::Setup()
{
	AddToViewport();
	bIsFocusable = true;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetWidgetToFocus(TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	UWorld* World = GetWorld();
	if(!ensure(World)) return;
    
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if(!ensure(PlayerController)) return;
    
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
	
	AFWPlayerCharacter* PlayerCharacter = PlayerController->GetPawn<AFWPlayerCharacter>();
	if(!ensure(PlayerCharacter)) return;

	InventoryComponent = PlayerCharacter->GetInventoryComponent();
	SetupInventoryDisplay();
	
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

void UFWInventoryWidget::UpdateDisplay(UFWInventoryItemWidget* NewItem)
{
	if(EquippedItem != nullptr)
	{
		EquippedItem->Unequip();
	}
	
	if(NewItem != nullptr)
	{
		NewItem->Equip();
		EquippedItem = NewItem;
	}
}
