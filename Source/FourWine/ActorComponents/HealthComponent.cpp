// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"


#include "FourWine/Interfaces/HealthComponentInterface.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 100;
	CurrentHealth = MaxHealth;
	Defenses = FDamageStruct(20,20,20);

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UHealthComponent::SetHealthValue(float InHealthValue)
{
	CurrentHealth = FMath::Clamp(InHealthValue, InHealthValue, MaxHealth);
}

bool UHealthComponent::TakeDamage(FDamageStruct const& DamageStruct, AController* CausingController,
	AActor* CausingActor)
{
	FDamageStruct const& CalculatedDamageStruct = Defenses.CalculateReduction(DamageStruct);

	const float DamageDealt = CalculatedDamageStruct.DamageSum();
	
	CurrentHealth -= DamageDealt;
	
	if(CurrentHealth <= 0)
	{
		
		if(IHealthComponentInterface* OwningInterface = Cast<IHealthComponentInterface>(GetOwner()))
			OwningInterface->HealthIsZero();
		
		return true;
	}
	return false;
}