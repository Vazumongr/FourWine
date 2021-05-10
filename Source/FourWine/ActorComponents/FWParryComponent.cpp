// Copyright Troy Records Jr 2021

#include "FWParryComponent.h"

#include "FourWine/FourWine.h"

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
			Wisp->GetRootComponent()->SetAbsolute(false, true, false);
			Wisp->SetActorLocation(GetOwner()->GetActorLocation());
		}
		else
		{
			UE_LOG(LogPlayer, Warning, TEXT("Wisp spawning failed"));
		}
	}
}

void UFWParryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	if(Wisp == nullptr) return;

	UpperBound = GetOwner()->GetActorLocation().Z + HeightVariance;
	LowerBound = GetOwner()->GetActorLocation().Z - HeightVariance;
	
	Angle += RotationSpeed * DeltaTime;
	XCord = Radius * FMath::Cos(Angle);
	YCord = Radius * FMath::Sin(Angle);
	if(bGoingUp)
	{
		ZCord += ElevationSpeed * DeltaTime;
	}
	else
	{
		ZCord -= ElevationSpeed * DeltaTime;
	}
	//ZCord = bGoingUp ? ElevationSpeed * DeltaTime : ElevationSpeed * DeltaTime * -1;
	const FVector ElevateVector{0,0,ZCord};
	//Wisp->SetActorLocation(Wisp->GetActorLocation() + ElevateVector);
	const FVector OrbitVector{XCord, YCord, 0};
			UE_LOG(LogPlayer, Warning, TEXT("AdjustmentVector:		%s"), *OrbitVector.ToString());

	UE_LOG(LogPlayer, Warning, TEXT("OwnerActorLocation:		%s"), *GetOwner()->GetActorLocation().ToString());
	Wisp->SetActorLocation(GetOwner()->GetActorLocation() + OrbitVector + ElevateVector);
	UE_LOG(LogPlayer, Warning, TEXT("WispActorLocation:		%s"), *Wisp->GetActorLocation().ToString());

	
	if((Wisp->GetActorLocation().Z > UpperBound && bGoingUp) || (Wisp->GetActorLocation().Z < LowerBound && !bGoingUp))
	{
		UE_LOG(LogTemp, Error, TEXT("FLIPPING!"));
		bGoingUp = !bGoingUp;
	}
}
