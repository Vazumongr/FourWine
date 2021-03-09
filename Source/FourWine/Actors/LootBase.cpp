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

// Called when the game starts or when spawned
void ALootBase::BeginPlay()
{
	Super::BeginPlay();

	if(DataAsset != nullptr)
	{
		TArray<UStaticMesh*> StaticMeshes = DataAsset->StaticMeshes;
        
        StaticMeshComponent->SetStaticMesh(StaticMeshes[FMath::RandRange(0, StaticMeshes.Num() - 1)]);
	}
	

	InventoryItem.DamageValues = FDamageStruct{10,10,10};
	//InventoryItem.ItemsClass = StaticClass();
	InventoryItem.StaticMesh = StaticMeshComponent->GetStaticMesh();
	
}

