// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"


#include "AbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "FourWine/Widgets/QuestJournalWidget.h"
#include "FourWine/Characters/FourWineCharacter.h"
#include "FourWine/PlayerStates/FWPlayerState.h"

void AMainPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindAction("OpenQuestJournal", IE_Pressed, this, &AMainPlayerController::OpenQuestJournal);
}

void AMainPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    PlayerCharacter = Cast<AFourWineCharacter>(InPawn);
    AFWPlayerState* PS = GetPlayerState<AFWPlayerState>();
    if(PS)
    {
        PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
    }
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
