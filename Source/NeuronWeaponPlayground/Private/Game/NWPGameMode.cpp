// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

// Class
#include "NWPGameMode.h"

// NWP
#include "NWPGameState.h"
#include "NWPHUD.h"

// UE
#include "UObject/ConstructorHelpers.h"

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

ANWPGameMode::ANWPGameMode(const class FObjectInitializer& ObjectInitializer)
	: Super()
{
	// Set default pawn class to our Blueprinted character
	// TODO: [NWP-REVIEW] Remove hard coded path
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Character/BP_Character"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// Use our custom NWP classes
	GameStateClass = ANWPGameState::StaticClass();
	HUDClass = ANWPHUD::StaticClass();
}
