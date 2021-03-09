// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestJournalWidget.h"

#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

#include "FourWine/ActorComponents/QuestManager.h"
#include "FourWine/Objects/QuestBase.h"
#include "FourWine/Widgets/QuestTitleWidget.h"


bool UQuestJournalWidget::Initialize()
{
    return Super::Initialize();
}

// Maybe break this up?
void UQuestJournalWidget::Setup(UQuestManager* InQuestManager)
{
    
    if(InQuestManager == nullptr) return;
    QuestManager = InQuestManager;
    
    UE_LOG(LogTemp, Warning, TEXT("Setup has been called"));

    const TArray<UQuestBase*>& Quests = QuestManager->GetQuests();
    
    for(UQuestBase* Quest : Quests)
    {
        //UE_LOG(LogTemp, Warning, TEXT("I found quest: %s"), *Quest->MyName);
        UQuestTitleWidget* TitleWidget = CreateWidget<UQuestTitleWidget>(this, QuestTitleWidgetClass);
        if(ensure(TitleWidget))
        {
            TitleWidget->QuestTitle->Text = FText::FromString(Quest->MyName);
            QuestTitlesScrollBox->AddChild(TitleWidget);
        }
        
        
        SetObjectiveList(Quest);
    }
    AddToViewport();
}

void UQuestJournalWidget::SetObjectiveList(UQuestBase* Quest)
{
    const TArray<FObjective>& Objectives = Quest->GetObjectives();
    for(const FObjective& Objective : Objectives)
    {
        UE_LOG(LogTemp, Warning, TEXT("Objective: %s"), *Objective.ObjectivesDescription);
        UQuestTitleWidget* ObjectiveWidget = CreateWidget<UQuestTitleWidget>(this, QuestTitleWidgetClass);
        ObjectiveWidget->QuestTitle->Text = FText::FromString(Objective.ObjectivesDescription);
        QuestDetailsScrollBox->AddChild(ObjectiveWidget);
    }
}