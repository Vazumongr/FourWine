// Copyright Troy Records Jr 2021

#include "FWParryComponent.h"

#include "FourWine/Characters/FWPlayerCharacter.h"
#include "FourWine/FourWine.h"
#include "Kismet/KismetSystemLibrary.h"

UFWParryComponent::UFWParryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFWParryComponent::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();
	if(World)
	{
		if(WispClass == nullptr)
		{
			Wisp = World->SpawnActor(AActor::StaticClass());
		}
		else
		{
			Wisp = World->SpawnActor(WispClass);
		}
		if(Wisp)
		{
			UE_LOG(LogPlayer, Warning, TEXT("Wisp spawning succeded"));
			Wisp->SetOwner(GetOwner());
			Wisp->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepRelativeTransform);
			UE_LOG(LogPlayer, Warning, TEXT("Owners location: %s"), *GetOwner()->GetActorLocation().ToString());
			Wisp->GetRootComponent()->SetAbsolute(true, true, false);
			Wisp->SetActorLocation(GetOwner()->GetActorLocation());
		}
		else
		{
			UE_LOG(LogPlayer, Warning, TEXT("Wisp spawning failed"));
		}
	}
}

void UFWParryComponent::Parry(bool InDebug)
{
	if(InDebug)
	{
		if(WispState != EWispState::Orbiting)
		{
			WispState = EWispState::Orbiting;
		}else if(WispState == EWispState::Orbiting)
		{
			WispState = EWispState::Lerping;
		}
	}
	else
	{
		if(ParryableActors.Num() > 0)
		{
			ParryTarget = ParryableActors[0];
			UE_LOG(LogWisp, Warning, TEXT("ParryTarget: %s"), *ParryTarget->GetName());
			WispState = EWispState::Lerping;
		}
	}
	
}

FVector UFWParryComponent::FindElevateVector(float DeltaTime)
{
	if(bGoingUp)
	{
		ZCord += ElevationSpeed * DeltaTime;
	}
	else
	{
		ZCord -= ElevationSpeed * DeltaTime;
	}
	return FVector{0,0,ZCord};
}

FVector UFWParryComponent::FindOrbitVector(float DeltaTime)
{
	Angle += RotationSpeed * DeltaTime;
	XCord = Radius * FMath::Cos(Angle);
	YCord = Radius * FMath::Sin(Angle);
	return FVector{XCord, YCord, 0};
}

void UFWParryComponent::Orbit(float DeltaTime)
{
	UpperBound = GetOwner()->GetActorLocation().Z + HeightVariance;
	LowerBound = GetOwner()->GetActorLocation().Z - HeightVariance;

	const FVector ElevateVector{FindElevateVector(DeltaTime)};

	const FVector OrbitVector{FindOrbitVector(DeltaTime)};

	Wisp->SetActorLocation(GetOwner()->GetActorLocation() + OrbitVector + ElevateVector);

	if((Wisp->GetActorLocation().Z > UpperBound && bGoingUp) || (Wisp->GetActorLocation().Z < LowerBound && !bGoingUp))
	{
		bGoingUp = !bGoingUp;
	}
}

void UFWParryComponent::Lerp(float DeltaTime)
{
	if(!bSetStartLocation)
	{
		FDateTime Time = FDateTime::Now();
		UE_LOG(LogWisp, Warning, TEXT("Start time: %s | %i"), *Time.ToString(), Time.GetMillisecond())
			
		StartLocation = Wisp->GetActorLocation();
		bSetStartLocation = true;
	}
	if(ParryTarget != nullptr)
	{
		DesiredLocation = ParryTarget->GetActorLocation();
	}
	else
	{
		DesiredLocation = Cast<AFWPlayerCharacter>(GetOwner())->DebugParryComponent->GetWispLocation();
	}
	
	
	DisplacementVector = DesiredLocation - GetOwner()->GetActorLocation();
	float Scalar = Radius / DisplacementVector.Size();
	DisplacementVector *= Scalar;
	FVector EndLocation = GetOwner()->GetActorLocation() + DisplacementVector;
	InterpAlpha += InterpSpeed * DeltaTime;
	Wisp->SetActorLocation(FMath::Lerp(StartLocation, EndLocation, InterpAlpha));
	
	if(InterpAlpha >= 1.f)	// We reached our target
	{
		WispState = EWispState::Resting;
		FDateTime Time = FDateTime::Now();
		UE_LOG(LogWisp, Warning, TEXT("End time: %s | %i"), *Time.ToString(), Time.GetMillisecond())
		const FVector WispLocation = Wisp->GetActorLocation();
		const FVector CharacterLocation = GetOwner()->GetActorLocation();
		RestLocationRelative = WispLocation - CharacterLocation;
		SpawnParryFX();
	}
	UKismetSystemLibrary::DrawDebugLine(this,GetOwner()->GetActorLocation(),DesiredLocation,FLinearColor::Blue,5.f,1);
}

void UFWParryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	if(Wisp == nullptr) return;

	if(WispState == EWispState::Orbiting)
	{
		Orbit(DeltaTime);
		bSetStartLocation = false;
		InterpAlpha = 0.f;
	}
	else if(WispState == EWispState::Lerping)
	{
		Lerp(DeltaTime);
	}
	else if(WispState == EWispState::Resting)
	{
		InterpAlpha = 0.f;
		Wisp->SetActorLocation(GetOwner()->GetActorLocation() + RestLocationRelative);
		//ParryableActors.Remove(ParryTarget);
		if(ParryableActors.Num()>0)
		{
			ParryableActors.RemoveAt(0);
			if(ParryableActors.Num()>0)
				ParryTarget = ParryableActors[0];
			else
				ParryTarget = nullptr;
		}
		else
		{
			WispState = EWispState::Orbiting;
            ParryTarget = nullptr;
		}
			
		//UE_LOG(LogWisp, Warning, TEXT("Removing"));
		
	}

	/*
	const FVector WispLocation = Wisp->GetActorLocation();
	const FVector CharacterLocation = GetOwner()->GetActorLocation();
	const FVector WispLocationRelative = WispLocation - CharacterLocation;
	UE_LOG(LogWisp, Error, TEXT("WispLocation:				%s"), *WispLocation.ToString());
	UE_LOG(LogWisp, Error, TEXT("CharacterLocation:			%s"), *CharacterLocation.ToString());
	UE_LOG(LogWisp, Error, TEXT("WispLocationRelative:		%s"), *WispLocationRelative.ToString());
	*/
	
	
}

FVector UFWParryComponent::GetWispLocation()
{
	return Wisp->GetActorLocation();
}
