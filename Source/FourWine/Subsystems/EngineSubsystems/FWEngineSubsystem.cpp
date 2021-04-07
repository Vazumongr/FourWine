// Copyright 2021 Troy Records Jr.


#include "FWEngineSubsystem.h"
#include "AbilitySystemGlobals.h"

void UFWEngineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UAbilitySystemGlobals::Get().InitGlobalData();
}