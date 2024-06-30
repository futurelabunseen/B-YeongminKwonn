// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArcadeProtoTypeGameMode.h"
#include "ArcadeProtoTypeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AArcadeProtoTypeGameMode::AArcadeProtoTypeGameMode()
{
	// set default pawn class to our Blueprinted character
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	// if (PlayerPawnBPClass.Class != NULL)
	// {
	// 	DefaultPawnClass = PlayerPawnBPClass.Class;
	// }
}
