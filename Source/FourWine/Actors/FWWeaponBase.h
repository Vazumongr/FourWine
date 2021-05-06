// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FourWine/DataTypes/GameStructs.h"

#include "FWWeaponBase.generated.h"



/**
 * 
 */
UCLASS()
class FOURWINE_API AFWWeaponBase : public AActor
{
	GENERATED_BODY()

	AFWWeaponBase();

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DoOverlap();
	void Setup(struct FInventoryItem InInventoryItem);
	FInventoryItem* StoreWeapon() { return &InventoryItem; }
	void StartAttack();
	void EndAttack();
	void SetOwningActor(AActor* InOwningActor);
	float GetDamage() const;
	
	FORCEINLINE TArray<TSubclassOf<class UFWGameplayAbility>>& GetAbilities() { return AbilitiesForOwner; };
	UStaticMesh* GetStaticMesh() const { return StaticMeshComponent->GetStaticMesh();}



protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Collision)
	class UBoxComponent* BoxCollider;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = StaticMeshActor)
	class UStaticMeshComponent* StaticMeshComponent;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = Stats)
	struct FDamageStruct DamageValues;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = Stats)
	struct FInventoryItem InventoryItem;

	

private:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess))
	TArray<AActor*> ActorsHitDuringThisAttack;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess), Category="FourWine|Abilities")
	TArray<TSubclassOf<class UFWGameplayAbility>> AbilitiesForOwner;

	bool bIsAttacking = false;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess))
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess))
	float CollisionRadius;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, meta = (AllowPrivateAccess))
	class AFWCharacterBase* OwningActor;
};
