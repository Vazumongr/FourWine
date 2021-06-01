// Troy Records Jr. 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FourWine/DataTypes/GameStructs.h"

#include "FWInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FOURWINE_API UFWInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFWInventoryComponent();
	
	// Called when the game starts
	virtual void BeginPlay() override;
	void AddItemToInventory(FInventoryItem InventoryItem);
	void AddLootToInventory(FLootData InLootData);
	bool GetInventoryItem(int32 Index, FInventoryItem& Item);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<struct FInventoryItem> Inventory;

		
};
