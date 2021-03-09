// Troy Records Jr. 2021


#include "InventoryComponent.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::AddItemToInventory(FInventoryItem InventoryItem)
{
	/*
	FInventoryItem InventoryItem;
	InventoryItem.DamageValues = DamageStruct;
	InventoryItem.ItemsClass = ItemClass;
	InventoryItem.StaticMesh = StaticMesh;
	*/
	Inventory.Add(InventoryItem);
}

bool UInventoryComponent::GetInventoryItem(int32 Index, FInventoryItem& InventoryItem)
{
	if(Index + 1 > Inventory.Num()) return false;
	InventoryItem = Inventory[Index];
	Inventory.RemoveAt(Index);
	return true;
}
