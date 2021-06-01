// Copyright 2021 Troy Records Jr.

#pragma once

#include "CoreMinimal.h"
#include "FourWine/Characters/FWCharacterBase.h"

#include "FWPlayerCharacter.generated.h"

/**
 * This class is for the player character. Right now, there is only one player
 * character planned. This class will only be subclassed via bp
 */
UCLASS()
class FOURWINE_API AFWPlayerCharacter : public AFWCharacterBase
{
	GENERATED_BODY()
	
public:
	AFWPlayerCharacter(const class FObjectInitializer& ObjectInitializer);
	
	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintCallable)
	class AFWWeaponBase* GetRightWeapon() const { return RightHandWeaponActor; }
	UFUNCTION(BlueprintCallable)
    class AFWWeaponBase* GetLeftWeapon() const { return LeftHandWeaponActor; }

	TArray<struct FInventoryItem>& GetInventory();

	FORCEINLINE class UFWInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns QuestManager subobject **/
	FORCEINLINE class UQuestManager* GetQuestManager() const { return QuestManager; }

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
protected:
	
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Client only
	virtual void OnRep_PlayerState() override;

	// Called from both SetupPlayerInputComponent and OnRep_PlayerState because of a potential race condition where the PlayerController might
	// call ClientRestart which calls SetupPlayerInputComponent before the PlayerState is repped to the client so the PlayerState would be null in SetupPlayerInputComponent.
	// Conversely, the PlayerState might be repped before the PlayerController calls ClientRestart so the Actor's InputComponent would be null in OnRep_PlayerState.
	void BindASCInput();

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
	
	void BoxTraceForPickUp();
	
	void EquipWeaponPressed();
	void EquipWeapon1();
	void EquipWeapon2();
	void EquipWeapon3();
	void Parry();
public:
	void EquipWeapon(int32 WeaponIdx);
protected:
	void CreateWeapon(struct FInventoryItem InventoryItem);

	void SetAttackPower();
	
	UFUNCTION(BlueprintCallable)
    FORCEINLINE bool IsAttacking() const { return bIsAttacking; };
	
	UFUNCTION(BlueprintCallable)
    void PrepareWeaponsForAttack() const;
	
	bool ASCInputBound = false;

	float LineTraceLength = 1000.f;

	UPROPERTY(EditAnywhere)
	float WalkSpeed = 600.f;
	UPROPERTY(EditAnywhere)
	float SprintModifier = 1.3f;
	bool bIsSprinting = false;
	UPROPERTY(BlueprintGetter=IsAttacking)
	bool bIsAttacking = false;		// We are in attack animation
	bool bCanChain = false;	// We can chain attacks
	uint8 AttackNumber = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bDoBinding = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bDebugParry = true;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess))
	class UNiagaraComponent* WeaponFXComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess))
	class UNiagaraSystem* WeaponFXSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = Weapons)
	class AFWWeaponBase* RightHandWeaponActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = Weapons)
	class AFWWeaponBase* LeftHandWeaponActor;

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** Follow camera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UFWInventoryComponent* InventoryComponent;
	
	/** Follow camera */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	class UQuestManager* QuestManager;
	
	/** Follow camera */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	class UFWParryComponent* ParryComponent;
public:
	/** Follow camera */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	class UFWParryComponent* DebugParryComponent;
};