// Copyright Troy Records Jr 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "FWParryComponent.generated.h"

/*
 *
 */
UCLASS()
class FOURWINE_API UFWParryComponent : public UActorComponent
{
	GENERATED_BODY()

	UFWParryComponent();
	
public:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
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
	
	
	UPROPERTY(VisibleAnywhere)
	AActor* Wisp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> WispClass;
};
