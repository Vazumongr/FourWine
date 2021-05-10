// Copyright Troy Records Jr 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "FWParryComponent.generated.h"

UENUM()
enum class EWispState : uint8
{
	Orbiting	= 0,
	Lerping		= 1,
	Resting		= 2
};

/*
 *
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FOURWINE_API UFWParryComponent : public UActorComponent
{
	GENERATED_BODY()

	UFWParryComponent();
	
public:

	virtual void BeginPlay() override;
	void Orbit(float DeltaTime);
	void Lerp(float DeltaTime);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FVector GetWispLocation();

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Angle = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RotationSpeed = 10.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ElevationSpeed = 10.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float XCord;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float YCord;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ZCord;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Radius = 50.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HeightVariance = 50.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float UpperBound;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float LowerBound;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bGoingUp = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bOrbiting = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bSetStartLocation = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float InterpSpeed = 10.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float InterpAlpha = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector StartLocation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector DisplacementVector;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector DesiredLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector RestLocationRelative;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EWispState WispState = EWispState::Orbiting;
	
	
	UPROPERTY(VisibleAnywhere)
	AActor* Wisp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> WispClass;
};
