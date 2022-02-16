// Troy Records Jr. 2021

#pragma once

#include "CoreMinimal.h"

//#include "UObject/NoExportTypes.h"
#include "GameStructs.generated.h"

UENUM()
enum class ELootType : uint8
{
	Default,
	Weapon,
	Armor,
	Consumable
};

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

	FString ToString()
	{
		return (FString::SanitizeFloat(Xetri) + ", " + FString::SanitizeFloat(Yourg) + ", " + FString::SanitizeFloat(Zadis));
	}

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

	FString ToString()
	{
		FString ReturnString = "";
		//ReturnString.Append(ItemsClass->GetDefaultObjectName() + " | ");
		ReturnString.Append(StaticMesh->GetName() + " | ");
		ReturnString.Append(DamageValues.ToString() + " | ");
		UEnum* Enum = StaticEnum<ELootType>();
		//Enum->GetNameStringByValue((uint8)LootType).ToLower().Replace(TEXT("_"), TEXT(" "));
		//Enum->GetNameStringByValue(*(int32*)LootType);
		//Enum->GetNameStringByValue(*Cast<uint64>(&LootType));
		//ReturnString.Append(Enum->GetNameStringByValue(*Cast<uint64>(&LootType)));
		ReturnString.Append(Enum->GetNameStringByValue((uint8)LootType).ToLower().Replace(TEXT("_"), TEXT(" ")));
		
		return ReturnString;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AFWWeaponBase> ItemsClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDamageStruct DamageValues;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELootType LootType = ELootType::Default;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLootData LootData;
	
	FGuid ItemID = FGuid::NewGuid();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
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