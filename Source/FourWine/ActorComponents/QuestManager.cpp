// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestManager.h"

#include "FourWine/Objects/QuestBase.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UQuestManager::UQuestManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	/*	THIS DOESNT LIKE WORKING IN CONSTRUCTORS
    UTestingObject* OrderMatter = CreateDefaultSubobject<UTestingObject>(TEXT("Quest"));
	OrderMatter->bOrderMatter = true;
	
	UTestingObject* OrderDontMatter = CreateDefaultSubobject<UTestingObject>(TEXT("Quest2"));
	*/
	
	
}

// Called when the game starts
void UQuestManager::BeginPlay()
{
	Super::BeginPlay();

	UQuestBase* OrderMatter = NewObject<UQuestBase>(this);
	OrderMatter->bOrderMatter = true;
	OrderMatter->SetQuestManager(this);
	OrderMatter->MyName = TEXT("OrderMatter");
	
	UQuestBase* OrderDontMatter = NewObject<UQuestBase>(this);
	OrderDontMatter->SetQuestManager(this);
	OrderDontMatter->MyName = TEXT("OrderDontMatter");

	// ...
	Quests.Add(OrderMatter);
	Quests.Add(OrderDontMatter);
	CurrentQuest = Quests[0];

	// ...
	
}

void UQuestManager::CheckForKillQuests(TSubclassOf<AActor> InClass)
{
	/*
	for(UTestingObject* Object : Quests)
	{
		Object->CheckKilledClass(InClass);
	}
	*/
	CurrentQuest->CheckKilledClass(InClass);
	PurgeQuests();
	
}

void UQuestManager::QuestComplete(UQuestBase* CompletedQuest)
{
	UE_LOG(LogTemp, Warning, TEXT("A QUEST has finished!"));
	UKismetSystemLibrary::PrintString(this, FString(TEXT("A QUEST has finished!")), true, true, FLinearColor::Green, 5);

	if(CompletedQuest != nullptr)
	{
		QuestToBePurged.Add(CompletedQuest);
	}
		
}

TArray<UQuestBase*> UQuestManager::GetQuests()
{
	return Quests;
}

void UQuestManager::PurgeQuests()
{
	for(UQuestBase* Object : QuestToBePurged)
	{
		Quests.Remove(Object);
	}
	QuestToBePurged.Empty();
	if(Quests.Num() > 0)
	{
		CurrentQuest = Quests[0];
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, FString(TEXT("All quests completed!")), true, true, FLinearColor::Green, 5);
		UE_LOG(LogTemp, Warning, TEXT("All quests completed!"));
	}
	
}
