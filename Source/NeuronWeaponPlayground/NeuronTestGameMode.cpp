// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "NeuronTestGameMode.h"
#include "NWPHUD.h"
#include "NeuronTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

ANeuronTestGameMode::ANeuronTestGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	// TODO: [NWP-REVIEW] Remove hard coded path
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Character/BP_Character"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ANWPHUD::StaticClass();
}

void ANeuronTestGameMode::BeginPlay()
{
	Super::BeginPlay();

	ObjectRegistry = NewObject<UNWPObjectRegistry>(this);
	ObjectRegistry->RegisterObject<ANeuronTestGameMode>(this);


	const TArray<ANeuronTestGameMode*>& List = ObjectRegistry->GetRegisteredObjects<ANeuronTestGameMode>();
}