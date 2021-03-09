// Troy Records Jr. 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FourWine/DataTypes/GameStructs.h"

#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FOURWINE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();
	
	// Called when the game starts
	virtual void BeginPlay() override;
	void AddItemToInventory(FInventoryItem InventoryItem);
	bool GetInventoryItem(int32 Index, FInventoryItem& Item);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<struct FInventoryItem> Inventory;
	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDamageStruct DamageStruct;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWeaponBase> ItemClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* StaticMesh;
	*/

		
};
