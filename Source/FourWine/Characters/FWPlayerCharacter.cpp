// Copyright 2021 Troy Records Jr.

#include "FWPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "FourWine/ActorComponents/QuestManager.h"
#include "FourWine/Abilities/FWAbilitySystemComponent.h"
#include "FourWine/Abilities/AttributeSets/FWAttributeSet.h"
#include "FourWine/Items/InventoryComponent.h"
#include "FourWine/PlayerStates/FWPlayerState.h"
#include "FourWine/Actors/LootBase.h"
#include "FourWine/Actors/FWWeaponBase.h"

AFWPlayerCharacter::AFWPlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
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
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));

	// Setup HealthComponent
	QuestManager = CreateDefaultSubobject<UQuestManager>(TEXT("Quest Manager"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	WeaponFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Weapon FX Component"));
	WeaponFXComponent->SetupAttachment(RootComponent);
}

void AFWPlayerCharacter::PossessedBy(AController* NewController)
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

void AFWPlayerCharacter::KillNotify(AActor* ActorKilled) const
{
	QuestManager->CheckForKillQuests(ActorKilled->GetClass());
}

/**
* On the Server, Possession happens before BeginPlay.
* On the Client, BeginPlay happens before Possession.
* So we can't use BeginPlay to do anything with the AbilitySystemComponent because we don't have it until the PlayerState replicates from possession.
*/
void AFWPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AFWPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up gameplay key bindings
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFWPlayerCharacter::ToggleSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFWPlayerCharacter::ToggleSprint);
	PlayerInputComponent->BindAction("SwitchShoulder", IE_Pressed, this, &AFWPlayerCharacter::SwitchShoulder);
	PlayerInputComponent->BindAction("OrientToMovement", IE_Pressed, this, &AFWPlayerCharacter::OrientToMovement);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AFWPlayerCharacter::Attack);
	PlayerInputComponent->BindAction("PickUpLoot", IE_Pressed, this, &AFWPlayerCharacter::EquipWeaponPressed);
	PlayerInputComponent->BindAction("EquipWeapon1", IE_Pressed, this, &AFWPlayerCharacter::EquipWeapon1);
	PlayerInputComponent->BindAction("EquipWeapon2", IE_Pressed, this, &AFWPlayerCharacter::EquipWeapon2);
	PlayerInputComponent->BindAction("EquipWeapon3", IE_Pressed, this, &AFWPlayerCharacter::EquipWeapon3);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFWPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFWPlayerCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &AFWPlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFWPlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &AFWPlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFWPlayerCharacter::LookUpAtRate);
	
		
	BindASCInput();
}

void AFWPlayerCharacter::OnRep_PlayerState()
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

void AFWPlayerCharacter::BindASCInput()
{
	if ( bDoBinding && !ASCInputBound && AbilitySystemComponent.IsValid() && IsValid(InputComponent))
	{
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
            FString("CancelTarget"), FString("EFWAbilityInputID"), static_cast<int32>(EFWAbilityInputID::Confirm), static_cast<int32>(EFWAbilityInputID::Cancel)));
		ASCInputBound = true;
	}
}

void AFWPlayerCharacter::MoveForward(float Value)
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

void AFWPlayerCharacter::MoveRight(float Value)
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

void AFWPlayerCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFWPlayerCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AFWPlayerCharacter::AttackEnd()
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

void AFWPlayerCharacter::ChainWindowOpen()
{
	bCanChain = true;
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, WeaponFXSystem, GetMesh()->GetSocketLocation(TEXT("weapon_r")));
}

void AFWPlayerCharacter::AddControllerYawInput(float Val)
{
	Super::AddControllerYawInput(Val * MouseSensitivity);
}

void AFWPlayerCharacter::AddControllerPitchInput(float Val)
{
	Super::AddControllerPitchInput(Val * MouseSensitivity);
}

void AFWPlayerCharacter::ToggleSprint()
{
	bIsSprinting = !bIsSprinting;
	if(bIsSprinting)
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * SprintModifier;
	else
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AFWPlayerCharacter::SwitchShoulder()
{
	const FVector CameraOffset = CameraBoom->SocketOffset;
	CameraBoom->SocketOffset = FVector(CameraOffset.X,CameraOffset.Y * -1, CameraOffset.Z);
}

void AFWPlayerCharacter::OrientToMovement()
{
	GetCharacterMovement()->bOrientRotationToMovement = !GetCharacterMovement()->bOrientRotationToMovement;
}

void AFWPlayerCharacter::Attack()
{
	if(RightHandWeaponActor == nullptr || LeftHandWeaponActor == nullptr || !AbilitySystemComponent.IsValid() || bDoBinding) return;
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

void AFWPlayerCharacter::PlayAttackAnim()
{
	PrepareWeaponsForAttack();
	switch(AttackNumber)
	{
	case 0:
		//PlayAnimMontage(FirstAttack);
		AbilitySystemComponent->AbilityLocalInputPressed(0);
		++AttackNumber;
		break;
	case 1:
		//PlayAnimMontage(SecondAttack);
		AbilitySystemComponent->AbilityLocalInputPressed(1);
		++AttackNumber;
		break;
	case 2:
		//PlayAnimMontage(ThirdAttack);
		AbilitySystemComponent->AbilityLocalInputPressed(2);
		AttackNumber = 0;
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Uh oh. Something went wrong."));
	}
}

void AFWPlayerCharacter::BoxTraceForPickUp()
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

void AFWPlayerCharacter::EquipWeaponPressed()
{
	BoxTraceForPickUp();
}

void AFWPlayerCharacter::EquipWeapon1()
{
	EquipWeapon(0);
}

void AFWPlayerCharacter::EquipWeapon2()
{
	EquipWeapon(1);
}

void AFWPlayerCharacter::EquipWeapon3()
{
	EquipWeapon(2);
}

void AFWPlayerCharacter::EquipWeapon(int32 WeaponIdx)
{
	if(RightHandWeaponActor != nullptr)
		InventoryComponent->AddItemToInventory(*RightHandWeaponActor->StoreWeapon());
	FInventoryItem InventoryItem;
	if(InventoryComponent->GetInventoryItem(WeaponIdx, InventoryItem))
	{
		CreateWeapon(InventoryItem);
		SetAttackPower();
	}
}

void AFWPlayerCharacter::CreateWeapon(FInventoryItem InventoryItem)
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

void AFWPlayerCharacter::SetAttackPower()
{
	if(RightHandWeaponActor != nullptr || AttributeSet.IsValid())
	{
		AttributeSet->SetAttackPower(RightHandWeaponActor->GetDamage());
	}
}

void AFWPlayerCharacter::PrepareWeaponsForAttack() const
{
	if(RightHandWeaponActor == nullptr || LeftHandWeaponActor == nullptr) return;
	RightHandWeaponActor->StartAttack();
	LeftHandWeaponActor->StartAttack();
}
