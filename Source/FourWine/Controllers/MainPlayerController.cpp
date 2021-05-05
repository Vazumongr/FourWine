// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"


#include "AbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "FourWine/Widgets/QuestJournalWidget.h"
#include "FourWine/Widgets/FWInventoryWidget.h"
#include "FourWine/Characters/FWPlayerCharacter.h"
#include "FourWine/PlayerStates/FWPlayerState.h"

void AMainPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindAction("OpenQuestJournal", IE_Pressed, this, &AMainPlayerController::OpenQuestJournal);
    InputComponent->BindAction("OpenInventory", IE_Pressed, this, &AMainPlayerController::OpenInventory);
}

void AMainPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    PlayerCharacter = Cast<AFWPlayerCharacter>(InPawn);
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

void AMainPlayerController::OpenInventory()
{
    if(InventoryWidget == nullptr)
    {
        InventoryWidget = CreateWidget<UFWInventoryWidget>(this, InventoryWidgetClass);
        InventoryWidget->SetInventoryReference(PlayerCharacter->GetInventory());
        InventoryWidget->Setup();
    }
    else
    {
        InventoryWidget->TearDown();
        InventoryWidget = nullptr;
    }
}
