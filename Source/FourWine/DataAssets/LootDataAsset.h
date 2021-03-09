// Troy Records Jr. 2021

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LootDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class FOURWINE_API ULootDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class UStaticMesh*> StaticMeshes;
	
};
