// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"



#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"
#include "FourWine/Characters/FourWineCharacter.h"
#include "FourWine/DataTypes/GameStructs.h"
#include "FourWine/Interfaces/HealthComponentInterface.h"

#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"

AWeaponBase::AWeaponBase()
{
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("Static Mesh Component")));
    RootComponent = StaticMeshComponent;
    
    
    BoxCollider = CreateDefaultSubobject<UBoxComponent>(FName(TEXT("Box Collider")));
    BoxCollider->SetupAttachment(RootComponent);
    
    BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::BeginOverlap);
    
    CollisionRadius = 50.f;
}

void AWeaponBase::DoOverlap_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("DoOverlap_Implementation being called on %s"), *GetName());
    if(GetOwner() == nullptr) return;
    if(OwningActor == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("OwningActor is not set on %s!"), *GetName());
        return;
    }
    
    FVector SphereLocation = StaticMeshComponent->GetSocketLocation(FName("CollisionSocket"));
    TArray<AActor*> ActorsHitOnThisTrace;   // These are the folk we do damage too!
    UKismetSystemLibrary::SphereOverlapActors(this, SphereLocation, CollisionRadius, ObjectTypes, nullptr, ActorsHitDuringThisAttack, ActorsHitOnThisTrace);
    ActorsHitDuringThisAttack.Append(ActorsHitOnThisTrace);
    UKismetSystemLibrary::DrawDebugSphere(this, SphereLocation, CollisionRadius, 12, FLinearColor::Green, 2.f);
    UAbilitySystemComponent* AbilitySystemComponent = OwningActor->GetAbilitySystemComponent();
    
    if(AbilitySystemComponent != nullptr)
    {
        FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(FName("Ability.Melee"));
        
        FGameplayEventData Payload;
        Payload.Instigator = this;
        Payload.Target = OwningActor;
        Payload.ContextHandle = AbilitySystemComponent->MakeEffectContext();
        Payload.EventTag = EventTag;
        Payload.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActorArray(ActorsHitOnThisTrace, false);
        
        
        AbilitySystemComponent->HandleGameplayEvent(EventTag, &Payload);
    }
    
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
    OwningActor = Cast<AFourWineCharacter>(InOwningActor);
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
    }
    else
    {
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
