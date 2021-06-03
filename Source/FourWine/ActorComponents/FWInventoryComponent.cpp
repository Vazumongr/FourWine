// Troy Records Jr. 2021


#include "FWInventoryComponent.h"

#include "FourWine/Actors/LootBase.h"
#include "FourWine/FourWine.h"


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

void UFWInventoryComponent::AddLootToInventory(FLootData InLootData)
{
	const FInventoryItem InventoryItem = FInventoryItem(InLootData);
	Inventory.Add(InventoryItem);
}

void UFWInventoryComponent::DropItemFromInventory(int32 Index)
{
	FInventoryItem InventoryItemToDrop = Inventory[Index];
	ALootBase* LootDrop = GetWorld()->SpawnActor<ALootBase>(ALootBase::StaticClass());
	if(LootDrop != nullptr)
	{
		UE_LOG(LogLoot, Warning, TEXT("LootDrop was successfully created and spawned!"));
		LootDrop->LootData = FStructConverter::InventoryItemToLootData(InventoryItemToDrop);
		UE_LOG(LogLoot, Warning, TEXT("LootDrop LootData: %s"), *LootDrop->LootData.ToString());
	}
	
	RemoveItemFromInventory(Index);
}

void UFWInventoryComponent::RemoveItemFromInventory(int32 Index)
{
	if(Index + 1 > Inventory.Num()) return;
	Inventory.RemoveAt(Index);
}

bool UFWInventoryComponent::GetInventoryItem(int32 Index, FInventoryItem& InventoryItem)
{
	if(Index + 1 > Inventory.Num()) return false;
	InventoryItem = Inventory[Index];
	InventoryItem.bIsEquipped = true;
	//Inventory.RemoveAt(Index);
	return true;
}
