// Troy Records Jr. 2021

#pragma once

#include "CoreMinimal.h"

//#include "UObject/NoExportTypes.h"
#include "GameStructs.generated.h"

USTRUCT(BlueprintType)
struct FDamageStruct
{
	GENERATED_BODY()

	FDamageStruct(float InX, float InY, float InZ)
	{
		Xetri = InX;
		Yourg = InY;
		Zadis = InZ;
	}
	FDamageStruct() { }

	/* Value coming in is damage being dealt.
	 * Values used locally is our resistances
	 */
	FDamageStruct CalculateReduction(const FDamageStruct &InDamage) const
	{
		FDamageStruct ReturnStruct;
		ReturnStruct.Xetri = InDamage.Xetri * (Xetri / 100);
		ReturnStruct.Yourg = InDamage.Yourg * (Yourg / 100);
		ReturnStruct.Zadis = InDamage.Zadis * (Zadis / 100);
		return ReturnStruct;
	}

	float DamageSum() const { return Xetri + Yourg + Zadis; }
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float Xetri = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float Yourg = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float Zadis = 0;
};

USTRUCT(BlueprintType)
struct FLootData
{
	GENERATED_BODY()

	FLootData() { StaticMesh = nullptr; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AFWWeaponBase> ItemsClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDamageStruct DamageValues;
};

USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()
	explicit FInventoryItem(FLootData InLootData)
	{
		LootData = InLootData;
	}

	FInventoryItem() { }

	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AFWWeaponBase> ItemsClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDamageStruct DamageValues;
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLootData LootData;
	
	FGuid ItemID = FGuid::NewGuid();
	bool bIsEquipped = false;
};

USTRUCT(BlueprintType)
struct FStructConverter
{
	GENERATED_BODY()

	static FLootData InventoryItemToLootData(FInventoryItem InItem)
	{
		return InItem.LootData;
	}

	static FInventoryItem LootDataToInventoryItem(FLootData InData)
	{
		return FInventoryItem(InData);
	}
};