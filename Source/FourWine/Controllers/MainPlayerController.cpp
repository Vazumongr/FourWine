// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "FourWine/Widgets/QuestJournalWidget.h"
#include "FourWine/FourWineCharacter.h"

void AMainPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindAction("OpenQuestJournal", IE_Pressed, this, &AMainPlayerController::OpenQuestJournal);
}

void AMainPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    PlayerCharacter = Cast<AFourWineCharacter>(InPawn);
}

void AMainPlayerController::OpenQuestJournal()
{
    UE_LOG(LogTemp, Warning, TEXT("Opening quest journal"));
    if(QuestJournalWidget == nullptr)
    {
        QuestJournalWidget = CreateWidget<UQuestJournalWidget>(this, QuestJournalWidgetClass);
        QuestJournalWidget->Setup(PlayerCharacter->GetQuestManager());
    }
    else
    {
        QuestJournalWidget->RemoveFromViewport();
        QuestJournalWidget = nullptr;
    }
    
}
