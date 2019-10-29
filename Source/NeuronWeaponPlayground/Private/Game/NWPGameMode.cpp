// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

// Class
#include "NWPGameMode.h"

// NWP
#include "NWPGameState.h"
#include "NWPHUD.h"
#include "NWPCharacter.h"

// UE
#include "UObject/ConstructorHelpers.h"

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

ANWPGameMode::ANWPGameMode(const class FObjectInitializer& ObjectInitializer)
	: Super()
{
	// Use our custom NWP classes
	GameStateClass = ANWPGameState::StaticClass();
	HUDClass = ANWPHUD::StaticClass();
	DefaultPawnClass = ANWPCharacter::StaticClass();
}
