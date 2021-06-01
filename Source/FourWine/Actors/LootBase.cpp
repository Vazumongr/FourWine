// Troy Records Jr. 2021


#include "LootBase.h"

#include "FourWine/DataAssets/LootDataAsset.h"

// Sets default values
ALootBase::ALootBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("Static Mesh Component")));
	SetRootComponent(StaticMeshComponent);
	

}

void ALootBase::PickUp(FInventoryItem& OutInventoryItem)
{
	OutInventoryItem = InventoryItem;
	Destroy();
}

void ALootBase::PickUp(FLootData& OutData)
{
	OutData = LootData;
	Destroy();
}

// Called when the game starts or when spawned
void ALootBase::BeginPlay()
{
	Super::BeginPlay();
	float x,y,z;
	if(bRandomLoot)
	{
		if(DataAsset != nullptr)
		{
			TArray<UStaticMesh*> StaticMeshes = DataAsset->StaticMeshes;
        
			StaticMeshComponent->SetStaticMesh(StaticMeshes[FMath::RandRange(0, StaticMeshes.Num() - 1)]);
		}
		x = FMath::RandRange(0,30);
		y = FMath::RandRange(0,30);
		z = FMath::RandRange(0,30);
	}
	InventoryItem.DamageValues = FDamageStruct{x,y,z};
	InventoryItem.StaticMesh = StaticMeshComponent->GetStaticMesh();
	
}

