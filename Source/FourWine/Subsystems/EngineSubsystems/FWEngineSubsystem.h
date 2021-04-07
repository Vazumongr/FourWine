// Copyright 2021 Troy Records Jr.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "FWEngineSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class FOURWINE_API UFWEngineSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};