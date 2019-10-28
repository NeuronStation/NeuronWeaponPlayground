// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

// Class
#include "NWPGameMode.h"

// NWP
#include "NWPHUD.h"

// UE
#include "UObject/ConstructorHelpers.h"

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

ANWPGameMode::ANWPGameMode()
	: Super()
{
	// Set default pawn class to our Blueprinted character
	// TODO: [NWP-REVIEW] Remove hard coded path
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Character/BP_Character"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ANWPHUD::StaticClass();
}
