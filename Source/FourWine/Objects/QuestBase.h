// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "QuestBase.generated.h"

UENUM()
enum class EObjectiveType : uint8
{
	KillObjective UMETA(DisplayName = "KillObjective"),
    FetchObjective UMETA(DisplayName = "FetchObjective")
};

USTRUCT()
struct FObjective
{
	GENERATED_BODY()

	UPROPERTY()
	uint32 UniqueID;

	FObjective()
	{
		static uint32 ID = 0;
		UniqueID = ID++;
		KillCounter = 0;
		KillTarget = 3;
		bIsCompleted = false;
		ObjectivesDescription = TEXT("I am objective.");
	}

	bool operator==(const FObjective& Type) const
	{
		return UniqueID == Type.UniqueID;
	}

	UPROPERTY(VisibleAnywhere)
	EObjectiveType ObjectiveType;
	UPROPERTY(VisibleAnywhere)
	uint8 KillCounter;
	UPROPERTY(VisibleAnywhere)
	uint8 KillTarget;
	UPROPERTY(VisibleAnywhere)
	bool bIsCompleted;
	UPROPERTY()
	FString ObjectivesDescription;
	UPROPERTY()
	FString ObjectiveName;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AActor>> ArrayOfTargets;
};

/**
 * 
 */
UCLASS()
class FOURWINE_API UQuestBase : public UObject
{
	GENERATED_BODY()

public:
	UQuestBase();
	
	void CheckKillObjective(FObjective& temp, TSubclassOf<AActor> InClass);

	void SetQuestManager(class UQuestManager* InQuestManager);

	// Bad name TODO rename to CheckForTargetKilled
	void CheckKilledClass(TSubclassOf<AActor> InClass);

	FORCEINLINE const TArray<struct FObjective>& GetObjectives() const { return Objectives; }

	FString MyName;
	
	bool bOrderMatter = false;
	
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<struct FObjective> Objectives;
	FObjective* CurrentObjective = nullptr;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UQuestManager* QuestManager;
	
};
