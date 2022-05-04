// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTSGameGameMode.h"
#include "Controller/RTSGamePlayerController.h"
#include "Actor/RTSGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARTSGameGameMode::ARTSGameGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ARTSGamePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}