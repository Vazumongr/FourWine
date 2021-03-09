// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetDummy.h"

#include "FourWine/ActorComponents/HealthComponent.h"

// Sets default values
ATargetDummy::ATargetDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Setup StaticMeshComponent
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	SetRootComponent(StaticMeshComponent);
	
	// Setup HealthComponent
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	HealthComponent->SetHealthValue(100);
}

// Called when the game starts or when spawned
void ATargetDummy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATargetDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ATargetDummy::TakeDamage(FDamageStruct const& DamageStruct, AController* CausingController, AActor* CausingActor)
{
	return HealthComponent->TakeDamage(DamageStruct, CausingController, CausingActor);
}

void ATargetDummy::HealthIsZero()
{
	Destroy();
}

