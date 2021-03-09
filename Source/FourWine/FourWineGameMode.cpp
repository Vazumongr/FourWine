// Copyright Epic Games, Inc. All Rights Reserved.

#include "FourWineGameMode.h"
#include "FourWineCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFourWineGameMode::AFourWineGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
