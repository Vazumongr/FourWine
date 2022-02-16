// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestBase.h"

#include "FourWine/ActorComponents/QuestManager.h"
#include "FourWine/Actors/TargetDummy.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

UQuestBase::UQuestBase()
{
	// Look into data tables for this. Should be able to do it that way instead of hard-coding
    ConstructorHelpers::FClassFinder<AActor> TargetActorClass(TEXT("/Game/Blueprints/BP_TargetDummy"));
    ConstructorHelpers::FClassFinder<AActor> TargetActorClass1(TEXT("/Game/Blueprints/BP_TargetDummy1"));
    
    FObjective Objective1;
    FObjective Objective2;
	
    Objective1.ArrayOfTargets.Add(TargetActorClass.Class);
    Objective1.ArrayOfTargets.Add(TargetActorClass1.Class);
    Objective1.ArrayOfTargets.Add(ATargetDummy::StaticClass());
    Objective1.ObjectiveType = EObjectiveType::KillObjective;
	Objective1.ObjectivesDescription = FString(TEXT("I am Objective1. Targets are TargetActorClass, TargetActorClass1, and TargetDummy"));
	Objective1.ObjectiveName = FString(TEXT("Objective1"));
	
    Objective2.ArrayOfTargets.Add(TargetActorClass.Class);
    Objective2.ArrayOfTargets.Add(TargetActorClass1.Class);
    Objective2.ObjectiveType = EObjectiveType::KillObjective;
	Objective2.ObjectivesDescription = FString(TEXT("I am Objective2. Targets are TargetActorClass and TargetActorClass1."));
	Objective2.ObjectiveName = FString(TEXT("Objective2"));

    Objectives.Add(Objective1);
    Objectives.Add(Objective2);

    CurrentObjective = &Objectives[0];
}

void UQuestBase::CheckKillObjective(FObjective& temp, TSubclassOf<AActor> InClass)
{
	UE_LOG(LogTemp, Warning, TEXT("%s BEING CHECKED"), *temp.ObjectiveName);
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("%s BEING CHECKED"), *temp.ObjectiveName), true, true, FLinearColor::Blue, 5);
	if(temp.bIsCompleted || temp.ObjectiveType != EObjectiveType::KillObjective) return;

	/*
	if(temp.ArrayOfTargets.Contains(InClass))
	{
		if(++temp.KillCounter == temp.KillTarget)
		{
			temp.bIsCompleted = true;
			UE_LOG(LogTemp, Warning, TEXT("%s COMPLETED"), *temp.ObjectiveName);
			
			const int32 NextObjectiveIndex = 1 + Objectives.IndexOfByPredicate([temp](const FObjective& Objective) { return Objective == temp; });
			UE_LOG(LogTemp, Warning, TEXT("Next objective index is: %d"), NextObjectiveIndex);
			//auto NextObjectiveIndex = Objectives.IndexOfByKey(temp) + 1;
			if(NextObjectiveIndex >= Objectives.Num())
			{
				// Quest is complete
				QuestManager->QuestComplete(this);
			}
			else
			{
				CurrentObjective = &Objectives[NextObjectiveIndex];
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("%s KILL COUNT INCREASED"), *temp.ObjectiveName);
	*/
	for( TSubclassOf<AActor> Class : temp.ArrayOfTargets)
	{
		if(InClass->IsChildOf(Class))
		{
			if(++temp.KillCounter == temp.KillTarget)
			{
			temp.bIsCompleted = true;
			UE_LOG(LogTemp, Warning, TEXT("You completed an objective"));
			UKismetSystemLibrary::PrintString(this, FString(TEXT("You completed an objective")), true, true, FLinearColor::Yellow, 5);
					
			const int32 NextObjectiveIndex = 1 + Objectives.IndexOfByPredicate([temp](const FObjective& Objective) { return Objective == temp; });
			UE_LOG(LogTemp, Warning, TEXT("Next objective index is: %d"), NextObjectiveIndex);
			//auto NextObjectiveIndex = Objectives.IndexOfByKey(temp) + 1;
			if(NextObjectiveIndex >= Objectives.Num())
			{
				// Quest is complete
				QuestManager->QuestComplete(this);
				}
					else
				{
					CurrentObjective = &Objectives[NextObjectiveIndex];
				}
			}
			UE_LOG(LogTemp, Warning, TEXT("Kill count increased"));
			//UKismetSystemLibrary::PrintString(this, FString(TEXT("Kill count increased")));
			break;
		}
			
	}
}

void UQuestBase::SetQuestManager(UQuestManager* InQuestManager)
{
	QuestManager = InQuestManager;
}

void UQuestBase::CheckKilledClass(TSubclassOf<AActor> InClass)
{
	
    if(bOrderMatter)
    {
        CheckKillObjective(*CurrentObjective, InClass);
    }
    else
    {
    	for(FObjective& temp : Objectives)
    	{
    		CheckKillObjective(temp, InClass);
    	}
    }
	
	/*
	for(FObjective& temp : Objectives)
	{
		CheckKillObjective(temp, InClass);
	}
	*/
}
