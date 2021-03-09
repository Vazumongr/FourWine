// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "Components/BoxComponent.h"
#include "FourWine/FourWineCharacter.h"
#include "FourWine/DataTypes/GameStructs.h"
#include "FourWine/Interfaces/HealthComponentInterface.h"

AWeaponBase::AWeaponBase()
{
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("Static Mesh Component")));
    RootComponent = StaticMeshComponent;
    
    
    BoxCollider = CreateDefaultSubobject<UBoxComponent>(FName(TEXT("Box Collider")));
    BoxCollider->SetupAttachment(RootComponent);
    
    BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::BeginOverlap);
}

void AWeaponBase::Setup(FInventoryItem InInventoryItem)
{
    DamageValues = InInventoryItem.DamageValues;
    StaticMeshComponent->SetStaticMesh(InInventoryItem.StaticMesh);
    InventoryItem = InInventoryItem;
}

void AWeaponBase::StartAttack()
{
    bIsAttacking = true;
    ActorsHitDuringThisAttack.Empty();
}

void AWeaponBase::EndAttack()
{
    bIsAttacking = false;
    ActorsHitDuringThisAttack.Empty();
}

void AWeaponBase::SetOwningActor(AActor* InOwningActor)
{
    OwningActor = InOwningActor;
}

void AWeaponBase::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
                               AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                               const FHitResult& SweepResult)
{
    if(!bIsAttacking) return;
    if(OtherActor == OwningActor) return;
    if(ActorsHitDuringThisAttack.Contains(OtherActor))
    {
        if(OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn()) return;
        GEngine->AddOnScreenDebugMessage(-1,2,FColor::Green,FString::Printf(TEXT("You already hit %s!"), *OtherActor->GetName()));
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1,2,FColor::Green,FString::Printf(TEXT("You're hitting %s!"), *OtherActor->GetName()));

        if(IHealthComponentInterface* OtherActorInterface = Cast<IHealthComponentInterface>(OtherActor))
        {
            ActorsHitDuringThisAttack.Add(OtherActor);
        
            if(OtherActorInterface->TakeDamage(DamageValues, GetWorld()->GetFirstPlayerController(), OwningActor))
            {
                if(AFourWineCharacter* PlayerCharacter = Cast<AFourWineCharacter>(OwningActor))
                {
                    PlayerCharacter->KillNotify(OtherActor);
                }
            }
        }
        
        
    }
}
