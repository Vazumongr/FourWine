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

	virtual void PossessedBy(AController* NewController) override;

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable)
	class AFWWeaponBase* GetRightWeapon() const { return RightHandWeaponActor; }
	UFUNCTION(BlueprintCallable)
	class AFWWeaponBase* GetLeftWeapon() const { return LeftHandWeaponActor; }

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
	bool ASCInputBound = false;
	bool bAbilitiesGranted = false;

	virtual void BeginPlay() override;
	
	virtual void PostInitializeComponents() override;

	// Client only
	virtual void OnRep_PlayerState() override;

	// Called from both SetupPlayerInputComponent and OnRep_PlayerState because of a potential race condition where the PlayerController might
	// call ClientRestart which calls SetupPlayerInputComponent before the PlayerState is repped to the client so the PlayerState would be null in SetupPlayerInputComponent.
	// Conversely, the PlayerState might be repped before the PlayerController calls ClientRestart so the Actor's InputComponent would be null in OnRep_PlayerState.
	void BindASCInput();

	

	TWeakObjectPtr<class UFWAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<class UFWAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="FourWine|Abilities")
	TArray<TSubclassOf<class UFWGameplayAbility>> DefaultAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="FourWine|Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	virtual void AddCharacterAbilities(TArray<TSubclassOf<class UFWGameplayAbility>>& AbilitiesToAdd);

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
	UFUNCTION(BlueprintCallable)
	void PrepareWeaponsForAttack() const;
	void BoxTraceForPickUp();
	void EquipWeaponPressed();
	void EquipWeapon1();
	void EquipWeapon2();
	void EquipWeapon3();
	void EquipWeapon(int32 WeaponIdx);
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
	class AFWWeaponBase* RightHandWeaponActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = Weapons)
	class AFWWeaponBase* LeftHandWeaponActor;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AFWWeaponBase> WeaponClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AFWWeaponBase> WeaponClass2;
	
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
