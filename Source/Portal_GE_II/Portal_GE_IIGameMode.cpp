// Copyright Epic Games, Inc. All Rights Reserved.

#include "Portal_GE_IIGameMode.h"
#include "Portal_GE_IIHUD.h"
#include "Portal_GE_IICharacter.h"
#include "UObject/ConstructorHelpers.h"

APortal_GE_IIGameMode::APortal_GE_IIGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = APortal_GE_IIHUD::StaticClass();
}
