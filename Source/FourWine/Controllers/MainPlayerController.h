// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FOURWINE_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	
protected:
    virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;

private:
	UFUNCTION()
	void OpenQuestJournal();
	
	UPROPERTY()
	class AFourWineCharacter* PlayerCharacter;

	UPROPERTY()
	class UQuestJournalWidget* QuestJournalWidget;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UQuestJournalWidget> QuestJournalWidgetClass;
	
};
