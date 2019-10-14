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

	const TArray<UObject*>& ListSeven = ObjectRegistry->GetRegisteredObjectsAsObjects<ANeuronTestGameMode>();

	const TArray<ANeuronTestGameMode*>& ListOne = ObjectRegistry->GetRegisteredObjects<ANeuronTestGameMode>(ANeuronTestGameMode::StaticClass());
	TArray<ANeuronTestGameMode*> ListTwo = ObjectRegistry->GetRegisteredObjectsCopy<ANeuronTestGameMode>(ANeuronTestGameMode::StaticClass());
	const TArray<ANeuronTestGameMode*>& ListThree = ObjectRegistry->GetRegisteredObjects<ANeuronTestGameMode>();
	TArray<ANeuronTestGameMode*> ListFour = ObjectRegistry->GetRegisteredObjectsCopy<ANeuronTestGameMode>();

	const TArray<UObject*>& ListFive = ObjectRegistry->GetRegisteredObjectsAsObjects(ANeuronTestGameMode::StaticClass());
	TArray<UObject*> ListSix = ObjectRegistry->GetRegisteredObjectsAsObjectsCopy(ANeuronTestGameMode::StaticClass());
	TArray<UObject*> ListEight = ObjectRegistry->GetRegisteredObjectsAsObjectsCopy<ANeuronTestGameMode>();
}