// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FourWine/Interfaces/HealthComponentInterface.h"
#include "FourWIne/DataTypes/GameStructs.h"

#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FOURWINE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	
	virtual bool TakeDamage(struct FDamageStruct const& DamageStruct, AController* CausingController, AActor* CausingActor);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Stats")
	float MaxHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Stats")
	float CurrentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Stats")
	FDamageStruct Defenses;

public:
	void SetHealthValue(float InHealthValue);
	float GetHealth() const { return CurrentHealth; }
	float GetMaxHealth() const { return MaxHealth; }

		
};
