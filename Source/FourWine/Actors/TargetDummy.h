// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FourWine/Interfaces/HealthComponentInterface.h"
#include "GameFramework/Actor.h"
#include "TargetDummy.generated.h"

UCLASS()
class FOURWINE_API ATargetDummy : public AActor, public IHealthComponentInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATargetDummy();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual bool TakeDamage(struct FDamageStruct const& DamageStruct, AController* CausingController, AActor* CausingActor) override;

	virtual void HealthIsZero() override;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats, meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* HealthComponent;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMeshComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	
};
