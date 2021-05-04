// Copyright Epic Games, Inc. All Rights Reserved.

#include "FourWineCharacter.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "FourWine/Abilities/AttributeSets/FWAttributeSet.h"
#include "FourWine/Abilities/FWAbilitySystemComponent.h"
#include "FourWine/ActorComponents/HealthComponent.h"
#include "FourWine/ActorComponents/QuestManager.h"
#include "FourWine/Actors/FWWeaponBase.h"
#include "FourWine/Actors/LootBase.h"
#include "FourWine/DataTypes/GameStructs.h"
#include "FourWine/Items/InventoryComponent.h"
#include "Fourwine/FourWine.h"
#include "FourWine/PlayerStates/FWPlayerState.h"
#include "FourWine/Abilities/FWGameplayAbility.h"

//////////////////////////////////////////////////////////////////////////
// AFourWineCharacter

AFourWineCharacter::AFourWineCharacter(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 15.f;
	BaseLookUpRate = 15.f;

	// set mouse sensitivity
	MouseSensitivity = 1.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxAcceleration = 10000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Setup HealthComponent
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	HealthComponent->SetHealthValue(100);
	
	// Setup HealthComponent
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));

	// Setup HealthComponent
	QuestManager = CreateDefaultSubobject<UQuestManager>(TEXT("Quest Manager"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	WeaponFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Weapon FX Component"));
	WeaponFXComponent->SetupAttachment(RootComponent);
}

// This is server only according to good ole Tranek
void AFourWineCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AFWPlayerState* PS = GetPlayerState<AFWPlayerState>();
	if(PS)
	{
		AbilitySystemComponent = Cast<UFWAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// This is because Ai won't have player controllers, so I guess we init here?? Don't see how this is relevant tbh.
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

		AttributeSet = PS->GetAttributeSet();

		// If we handle players disconnecting and rejoining in the future, we'll have to change this so that possession from rejoining doesn't reset attributes.
		// For now, we assume possession = spawn/respawn
		InitializeAttributes();

		AddCharacterAbilities(DefaultAbilities);
	}
}

UAbilitySystemComponent* AFourWineCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFourWineCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up gameplay key bindings
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFourWineCharacter::ToggleSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFourWineCharacter::ToggleSprint);
	PlayerInputComponent->BindAction("SwitchShoulder", IE_Pressed, this, &AFourWineCharacter::SwitchShoulder);
	PlayerInputComponent->BindAction("OrientToMovement", IE_Pressed, this, &AFourWineCharacter::OrientToMovement);
	//PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AFourWineCharacter::Attack);
	PlayerInputComponent->BindAction("PickUpLoot", IE_Pressed, this, &AFourWineCharacter::EquipWeaponPressed);
	PlayerInputComponent->BindAction("EquipWeapon1", IE_Pressed, this, &AFourWineCharacter::EquipWeapon1);
	PlayerInputComponent->BindAction("EquipWeapon2", IE_Pressed, this, &AFourWineCharacter::EquipWeapon2);
	PlayerInputComponent->BindAction("EquipWeapon3", IE_Pressed, this, &AFourWineCharacter::EquipWeapon3);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFourWineCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFourWineCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &AFourWineCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFourWineCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &AFourWineCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFourWineCharacter::LookUpAtRate);
	
		
	BindASCInput();
}

/**
* On the Server, Possession happens before BeginPlay.
* On the Client, BeginPlay happens before Possession.
* So we can't use BeginPlay to do anything with the AbilitySystemComponent because we don't have it until the PlayerState replicates from possession.
*/
void AFourWineCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AFourWineCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AFourWineCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AFWPlayerState* PS = GetPlayerState<AFWPlayerState>();
	if(PS)
	{
		
		AbilitySystemComponent = Cast<UFWAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		AbilitySystemComponent->InitAbilityActorInfo(PS, this);
		
		BindASCInput();
		

		AttributeSet = PS->GetAttributeSet();

		InitializeAttributes();
	}
}

void AFourWineCharacter::BindASCInput()
{
	if (/*!ASCInputBound && */AbilitySystemComponent.IsValid() && IsValid(InputComponent))
    {
    	AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
    		FString("CancelTarget"), FString("EFWAbilityInputID"), static_cast<int32>(EFWAbilityInputID::Confirm), static_cast<int32>(EFWAbilityInputID::Cancel)));
    	ASCInputBound = true;
    }
}

void AFourWineCharacter::AddCharacterAbilities(TArray<TSubclassOf<class UFWGameplayAbility>>& AbilitiesToAdd)
{
	if(GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || bAbilitiesGranted)
	{
		return;
	}
	
	for(TSubclassOf<UFWGameplayAbility>& Ability : AbilitiesToAdd)
	{
		AbilitySystemComponent->GiveAbility(
            FGameplayAbilitySpec(Ability, 1, static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID), this));
		bAbilitiesGranted = true;
	}
}

void AFourWineCharacter::InitializeAttributes()
{
	if(!AbilitySystemComponent.IsValid())
	{
		UE_LOG(LogAttribute, Warning, TEXT("ASC is not valid"));
		return;
	}
	if(!DefaultAttributes)
	{
		UE_LOG(LogAttribute, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, 1, EffectContextHandle);
	if(NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}

void AFourWineCharacter::SetHealth(float Health)
{
	if(AttributeSet.IsValid())
	{
		AttributeSet->SetHealth(Health);
	}
}

void AFourWineCharacter::SetStamina(float Stamina)
{
	if(AttributeSet.IsValid())
	{
		AttributeSet->SetStamina(Stamina);
	}
}

#pragma region [MOVEMENT]
void AFourWineCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFourWineCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AFourWineCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AFourWineCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
		
	}
}

void AFourWineCharacter::ToggleSprint()
{
	bIsSprinting = !bIsSprinting;
	if(bIsSprinting)
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * SprintModifier;
	else
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		
}

void AFourWineCharacter::SwitchShoulder()
{
	const FVector CameraOffset = CameraBoom->SocketOffset;
	CameraBoom->SocketOffset = FVector(CameraOffset.X,CameraOffset.Y * -1, CameraOffset.Z);
}
#pragma endregion

void AFourWineCharacter::OrientToMovement()
{
	GetCharacterMovement()->bOrientRotationToMovement = !GetCharacterMovement()->bOrientRotationToMovement;
}

void AFourWineCharacter::KillNotify(AActor* ActorKilled) const
{
	//UE_LOG(LogTemp, Warning, TEXT("You killed: %s"), *ActorKilled->GetName());
	QuestManager->CheckForKillQuests(ActorKilled->GetClass());
}

float AFourWineCharacter::GetHealth() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetHealth();
	}

	return 0.0f;
}

float AFourWineCharacter::GetMaxHealth() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxHealth();
	}

	return 0.0f;
}

float AFourWineCharacter::GetStamina() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetStamina();
	}

	return 0.0f;
}

float AFourWineCharacter::GetMaxStamina() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxStamina();
	}

	return 0.0f;
}

float AFourWineCharacter::GetAttackPower() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetAttackPower();
	}

	return 0.0f;
}

void AFourWineCharacter::AttackEnd()
{
	bIsAttacking = false;
	bCanChain = false;
	AttackNumber = 0;
	if(LeftHandWeaponActor == nullptr || RightHandWeaponActor == nullptr)
	{
		return;
	}
	RightHandWeaponActor->EndAttack();
	LeftHandWeaponActor->EndAttack();
}

void AFourWineCharacter::ChainWindowOpen()
{
	bCanChain = true;
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, WeaponFXSystem, GetMesh()->GetSocketLocation(TEXT("weapon_r")));
}

void AFourWineCharacter::AddControllerYawInput(float Val)
{
	Super::AddControllerYawInput(Val * MouseSensitivity);
}

void AFourWineCharacter::AddControllerPitchInput(float Val)
{
	Super::AddControllerPitchInput(Val * MouseSensitivity);
}

// TODO refactor this pos
void AFourWineCharacter::Attack()
{
	if(RightHandWeaponActor == nullptr || LeftHandWeaponActor == nullptr) return;
	if(bIsAttacking)
	{
		if(bCanChain)
		{
			PlayAttackAnim();
			bCanChain = false;
		}
	}
	else
	{
		PlayAttackAnim();
		bIsAttacking = true;
	}
}

void AFourWineCharacter::PlayAttackAnim()
{
	PrepareWeaponsForAttack();
	switch(AttackNumber)
	{
	case 0:
		PlayAnimMontage(FirstAttack);
		++AttackNumber;
		break;
	case 1:
		PlayAnimMontage(SecondAttack);
		++AttackNumber;
		break;
	case 2:
		PlayAnimMontage(ThirdAttack);
		AttackNumber = 0;
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Uh oh. Something went wrong."));
	}
}

void AFourWineCharacter::PrepareWeaponsForAttack() const
{
	if(RightHandWeaponActor == nullptr || LeftHandWeaponActor == nullptr) return;
	RightHandWeaponActor->StartAttack();
	LeftHandWeaponActor->StartAttack();
}


void AFourWineCharacter::EquipWeaponPressed()
{
	BoxTraceForPickUp();
}

void AFourWineCharacter::EquipWeapon1()
{
	EquipWeapon(0);
	/*
	if(RightHandWeaponActor != nullptr)
		InventoryComponent->AddItemToInventory(*RightHandWeaponActor->StoreWeapon());
	FInventoryItem InventoryItem;
	if(InventoryComponent->GetInventoryItem(0, InventoryItem))
		CreateWeapon(InventoryItem);
	*/
}

void AFourWineCharacter::EquipWeapon2()
{
	EquipWeapon(1);
	/*
	if(RightHandWeaponActor != nullptr)
		InventoryComponent->AddItemToInventory(*RightHandWeaponActor->StoreWeapon());
	FInventoryItem InventoryItem;
	if(InventoryComponent->GetInventoryItem(1, InventoryItem))
		CreateWeapon(InventoryItem);
	*/
}

void AFourWineCharacter::EquipWeapon3()
{
	EquipWeapon(2);
	/*
	if(RightHandWeaponActor != nullptr)
		InventoryComponent->AddItemToInventory(*RightHandWeaponActor->StoreWeapon());
	FInventoryItem InventoryItem;
	if(InventoryComponent->GetInventoryItem(2, InventoryItem))
		CreateWeapon(InventoryItem);
	*/
}

void AFourWineCharacter::EquipWeapon(int32 WeaponIdx)
{
	if(RightHandWeaponActor != nullptr)
		InventoryComponent->AddItemToInventory(*RightHandWeaponActor->StoreWeapon());
	FInventoryItem InventoryItem;
	if(InventoryComponent->GetInventoryItem(WeaponIdx, InventoryItem))
		CreateWeapon(InventoryItem);
}

void AFourWineCharacter::BoxTraceForPickUp()
{
	const FVector Start = FollowCamera->GetComponentLocation();
	const FVector End = Start + FollowCamera->GetForwardVector() * LineTraceLength;
	const FVector HalfSize = FVector(5,5,5);
	const FRotator Orientation = FRotator::ZeroRotator;
	const TArray<AActor*> ActorsToIgnore;
	FHitResult OutHit;

	UKismetSystemLibrary::BoxTraceSingle(this, Start, End, HalfSize, Orientation, UEngineTypes::ConvertToTraceType(ECC_Camera), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, true);

	if(OutHit.bBlockingHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("We hit %s "), *OutHit.Actor->GetName());

		TWeakObjectPtr<ALootBase> LootActor = Cast<ALootBase>(OutHit.Actor);
		if(LootActor.IsValid())
		{
			//CreateWeapon(LootActor->PickUp());
			FInventoryItem InventoryItem;
			LootActor->PickUp(InventoryItem);
			InventoryComponent->AddItemToInventory(InventoryItem);
		}
	}
}

void AFourWineCharacter::CreateWeapon(FInventoryItem InventoryItem)
{
	const TSubclassOf<AFWWeaponBase> SpawningClass = InventoryItem.ItemsClass;
	
	if(LeftHandWeaponActor != nullptr)
		LeftHandWeaponActor->Destroy();
	if(RightHandWeaponActor != nullptr)
		RightHandWeaponActor->Destroy();
	AbilitySystemComponent->ClearAllAbilities();

	RightHandWeaponActor = GetWorld()->SpawnActor<AFWWeaponBase>(SpawningClass);
	RightHandWeaponActor->SetOwner(this);
	RightHandWeaponActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("weapon_r"));
	RightHandWeaponActor->SetActorRelativeRotation(FRotator(0,0,270));
	RightHandWeaponActor->Setup(InventoryItem);
	RightHandWeaponActor->SetOwningActor(this);
	AddCharacterAbilities(RightHandWeaponActor->GetAbilities());
	
	LeftHandWeaponActor = GetWorld()->SpawnActor<AFWWeaponBase>(SpawningClass);
	LeftHandWeaponActor->SetOwner(this);
	LeftHandWeaponActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("weapon_l"));
	LeftHandWeaponActor->SetActorRelativeRotation(FRotator(0,0,90));
	LeftHandWeaponActor->Setup(InventoryItem);
	LeftHandWeaponActor->SetOwningActor(this);
}