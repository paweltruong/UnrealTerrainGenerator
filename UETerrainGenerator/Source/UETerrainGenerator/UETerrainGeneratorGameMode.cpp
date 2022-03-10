// Copyright Epic Games, Inc. All Rights Reserved.

#include "UETerrainGeneratorGameMode.h"
#include "UETerrainGeneratorCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUETerrainGeneratorGameMode::AUETerrainGeneratorGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
