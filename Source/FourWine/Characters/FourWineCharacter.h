// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"

#include "FourWineCharacter.generated.h"

UCLASS()
class FOURWINE_API AFourWineCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* HealthComponent;
	
	/** Follow camera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* InventoryComponent;
	
	/** Follow camera */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	class UQuestManager* QuestManager;
public:
	AFourWineCharacter(const class FObjectInitializer& ObjectInitializer);

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void KillNotify(AActor* ActorKilled) const;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;
	
	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Camera)
	float MouseSensitivity;

	UFUNCTION(BlueprintCallable, Category = "FourWine|FWPlayerState|Attributes")
    float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "FourWine|FWPlayerState|Attributes")
    float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "FourWine|FWPlayerState|Attributes")
    float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "FourWine|FWPlayerState|Attributes")
    float GetMaxStamina() const;

	UFUNCTION(BlueprintCallable, Category = "FourWine|FWPlayerState|Attributes")
    float GetAttackPower() const;
	

protected:

	virtual void BeginPlay() override;

	TWeakObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<class UFWAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="FourWine|Abilities")
	TArray<TSubclassOf<class UGameplayAbility>> DefaultAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="FourWine|Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	virtual void AddCharacterAbilities();

	virtual void InitializeAttributes();

	virtual void SetHealth(float Health);
	virtual void SetStamina(float Stamina);

	float LineTraceLength = 1000.f;

#pragma region [MOVEMENT]
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);
	
#pragma endregion [MOVEMENT]

	// Lets us know the attack is over and the chain window closes
	UFUNCTION(BlueprintCallable)
    void AttackEnd();
	// Lets us know the chain window has opened
	UFUNCTION(BlueprintCallable)
    void ChainWindowOpen();
    
	virtual void AddControllerYawInput(float Val) override;
	
    virtual void AddControllerPitchInput(float Val) override;

	void ToggleSprint();
	void SwitchShoulder();
	void OrientToMovement();
	void Attack();
	void PlayAttackAnim();
	void PrepareWeaponsForAttack() const;
	void BoxTraceForPickUp();
	void EquipWeaponPressed();
	void EquipWeapon1();
	void EquipWeapon2();
	void EquipWeapon3();
	void CreateWeapon(struct FInventoryItem InventoryItem);
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
	UPROPERTY(EditAnywhere)
	float WalkSpeed = 600.f;
	UPROPERTY(EditAnywhere)
	float SprintModifier = 1.3f;
	UPROPERTY(EditAnywhere)
	UAnimMontage* FirstAttack;
	UPROPERTY(EditAnywhere)
	UAnimMontage* SecondAttack;
	UPROPERTY(EditAnywhere)
	UAnimMontage* ThirdAttack;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAnimInstance> AnimInstance;
	bool bIsSprinting = false;
	UPROPERTY(BlueprintGetter=IsAttacking)
	bool bIsAttacking = false;		// We are in attack animation
	bool bCanChain = false;	// We can chain attacks
	uint8 AttackNumber = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess))
	class UNiagaraComponent* WeaponFXComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess))
	class UNiagaraSystem* WeaponFXSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = Weapons)
	class AWeaponBase* RightHandWeaponActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = Weapons)
	class AWeaponBase* LeftHandWeaponActor;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AWeaponBase> WeaponClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AWeaponBase> WeaponClass2;
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsAttacking() const { return bIsAttacking; };

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns QuestManager subobject **/
	FORCEINLINE UQuestManager* GetQuestManager() const { return QuestManager; }
};