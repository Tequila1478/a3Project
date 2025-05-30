// Copyright Epic Games, Inc. All Rights Reserved.

#include "a3ProjectGameMode.h"
#include "a3ProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

Aa3ProjectGameMode::Aa3ProjectGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
