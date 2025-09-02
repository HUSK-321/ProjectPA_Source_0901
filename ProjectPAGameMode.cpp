// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectPAGameMode.h"
#include "ProjectPACharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectPAGameMode::AProjectPAGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
