// Troy Records Jr. 2021

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FourWine/DataTypes/GameStructs.h"

#include "LootBase.generated.h"

UCLASS()
class FOURWINE_API ALootBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALootBase();

	void PickUp(FInventoryItem& InventoryItem);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInventoryItem InventoryItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ULootDataAsset* DataAsset;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
