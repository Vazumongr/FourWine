// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FourWine/Objects/QuestBase.h"

#include "QuestJournalWidget.generated.h"

/**
 * 
 */
UCLASS()
class FOURWINE_API UQuestJournalWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual bool Initialize() override;
	void SetObjectiveList(UQuestBase* Quest);

	void Setup(class UQuestManager* InQuestManager);

private:
	UPROPERTY()
	UQuestManager* QuestManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TSubclassOf<class UQuestTitleWidget> QuestTitleWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	class UScrollBox* QuestTitlesScrollBox;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	class UScrollBox* QuestDetailsScrollBox;
	
};
