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
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AWeaponBase> ItemsClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDamageStruct DamageValues;
};