// Copyright Epic Games, Inc. All Rights Reserved.

#include "GhostGameMode.h"
#include "GhostCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGhostGameMode::AGhostGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<AGhostCharacter> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Succeeded())
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
