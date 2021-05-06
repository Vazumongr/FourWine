// Fill out your copyright notice in the Description page of Project Settings.


#include "FWPlayerController.h"


#include "AbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "FourWine/Widgets/QuestJournalWidget.h"
#include "FourWine/Widgets/FWInventoryWidget.h"
#include "FourWine/Characters/FWPlayerCharacter.h"
#include "FourWine/PlayerStates/FWPlayerState.h"

void AFWPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindAction("OpenQuestJournal", IE_Pressed, this, &AFWPlayerController::OpenQuestJournal);
    InputComponent->BindAction("OpenInventory", IE_Pressed, this, &AFWPlayerController::OpenInventory);
}

void AFWPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    PlayerCharacter = Cast<AFWPlayerCharacter>(InPawn);
    AFWPlayerState* PS = GetPlayerState<AFWPlayerState>();
    if(PS)
    {
        PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
    }
}

void AFWPlayerController::OpenQuestJournal()
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

void AFWPlayerController::OpenInventory()
{
    if(InventoryWidget == nullptr)  // Wasn't made yet, so make it.
    {
        InventoryWidget = CreateWidget<UFWInventoryWidget>(this, InventoryWidgetClass);
        InventoryWidget->Setup();
    }
    else
    {
        InventoryWidget->TearDown();
        InventoryWidget = nullptr;
    }
    /* TODO make to toggle eventually
    if(InventoryWidget->IsVisible())
    {
        // Make invisible
        InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
    else
    {
        // Make visible
        InventoryWidget->SetVisibility(ESlateVisibility::Visible);
    }
    */
}
