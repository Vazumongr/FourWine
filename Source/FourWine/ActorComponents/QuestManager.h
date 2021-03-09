// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "QuestManager.generated.h"




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FOURWINE_API UQuestManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestManager();

	void CheckForKillQuests(TSubclassOf<AActor> InClass);

	void QuestComplete(class UQuestBase* CompletedQuest);

	TArray<UQuestBase*> GetQuests();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void PurgeQuests();

	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<UQuestBase*> Quests;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<UQuestBase*> QuestToBePurged;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UQuestBase* CurrentQuest;
};
