// Troy Records Jr. 2021


#include "FWInventoryComponent.h"


// Sets default values for this component's properties
UFWInventoryComponent::UFWInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UFWInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UFWInventoryComponent::AddItemToInventory(FInventoryItem InventoryItem)
{
	Inventory.Add(InventoryItem);
}

bool UFWInventoryComponent::GetInventoryItem(int32 Index, FInventoryItem& InventoryItem)
{
	if(Index + 1 > Inventory.Num()) return false;
	InventoryItem = Inventory[Index];
	Inventory.RemoveAt(Index);
	return true;
}
