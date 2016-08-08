// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "SacredHeart.h"
#include "SacredHeartGameMode.h"
#include "SacredHeartHUD.h"
#include "SacredHeartCharacter.h"

ASacredHeartGameMode::ASacredHeartGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ASacredHeartHUD::StaticClass();

}
