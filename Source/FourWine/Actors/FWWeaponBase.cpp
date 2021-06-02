// Fill out your copyright notice in the Description page of Project Settings.


#include "FWWeaponBase.h"
#include "FourWine/FourWine.h"


#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"
#include "FourWine/Characters/FWPlayerCharacter.h"
#include "FourWine/DataTypes/GameStructs.h"
#include "FourWine/Interfaces/HealthComponentInterface.h"

#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"

AFWWeaponBase::AFWWeaponBase()
{
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("Static Mesh Component")));
    RootComponent = StaticMeshComponent;
    
    
    BoxCollider = CreateDefaultSubobject<UBoxComponent>(FName(TEXT("Box Collider")));
    BoxCollider->SetupAttachment(RootComponent);
    
    BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AFWWeaponBase::BeginOverlap);
    
    CollisionRadius = 50.f;
}

void AFWWeaponBase::DoOverlap_Implementation()
{
    UE_LOG(LogWeapon, Display, TEXT("DoOverlap_Implementation being called on %s"), *GetName());
    if(GetOwner() == nullptr) return;
    if(OwningActor == nullptr)
    {
        UE_LOG(LogWeapon, Warning, TEXT("OwningActor is not set on %s!"), *GetName());
        //return;
    }
    
    FVector SphereLocation = StaticMeshComponent->GetSocketLocation(FName("CollisionSocket"));
    TArray<AActor*> ActorsHitOnThisTrace;   // These are the folk we do damage too!
    ActorsHitDuringThisAttack.Add(OwningActor);
    UKismetSystemLibrary::SphereOverlapActors(this, SphereLocation, CollisionRadius, ObjectTypes, nullptr, ActorsHitDuringThisAttack, ActorsHitOnThisTrace);
    ActorsHitDuringThisAttack.Append(ActorsHitOnThisTrace);
    if(OwningActor->bDebugMelee)
        UKismetSystemLibrary::DrawDebugSphere(this, SphereLocation, CollisionRadius, 12, FLinearColor::Green, 2.f);
    UAbilitySystemComponent* AbilitySystemComponent;
    if(OwningActor != nullptr)
        AbilitySystemComponent = OwningActor->GetAbilitySystemComponent();
    else
        return;
        
    
    if(AbilitySystemComponent != nullptr)
    {
        FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(FName("Ability.Melee"));
        FGameplayEventData Payload;
         
        Payload.Instigator = this;
        Payload.ContextHandle = AbilitySystemComponent->MakeEffectContext();
        Payload.EventTag = EventTag;
        Payload.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActorArray(ActorsHitOnThisTrace, false);

        for(AActor* Act : ActorsHitOnThisTrace)
        {
            //UE_LOG(LogPlayer, Warning, TEXT("hit %s"), *Act->GetName());
        }
        
        AbilitySystemComponent->HandleGameplayEvent(EventTag, &Payload);
    }
    
}

void AFWWeaponBase::Setup(FInventoryItem InInventoryItem)
{
    DamageValues = InInventoryItem.LootData.DamageValues;
    StaticMeshComponent->SetStaticMesh(InInventoryItem.LootData.StaticMesh);
    InventoryItem = InInventoryItem;
}

void AFWWeaponBase::StartAttack()
{
    bIsAttacking = true;
    ActorsHitDuringThisAttack.Empty();
}

void AFWWeaponBase::EndAttack()
{
    bIsAttacking = false;
    ActorsHitDuringThisAttack.Empty();
}

void AFWWeaponBase::SetOwningActor(AActor* InOwningActor)
{
    OwningActor = Cast<AFWPlayerCharacter>(InOwningActor);
}

float AFWWeaponBase::GetDamage() const
{
    return DamageValues.DamageSum();
}

void AFWWeaponBase::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
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
                if(AFWPlayerCharacter* PlayerCharacter = Cast<AFWPlayerCharacter>(OwningActor))
                {
                    PlayerCharacter->KillNotify(OtherActor);
                }
            }
        }
        
        
    }
}
