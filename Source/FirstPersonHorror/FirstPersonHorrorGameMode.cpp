// Copyright Epic Games, Inc. All Rights Reserved.

#include "FirstPersonHorrorGameMode.h"
#include "FirstPersonHorrorHUD.h"
#include "FirstPersonHorrorCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFirstPersonHorrorGameMode::AFirstPersonHorrorGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFirstPersonHorrorHUD::StaticClass();
}
