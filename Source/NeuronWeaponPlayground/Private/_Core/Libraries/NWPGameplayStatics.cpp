// Copyright 2019 Neuron Station. All Rights Reserved.

// Class
#include "NWPGameplayStatics.h"

// NWP
#include"NWPGameMode.h"
#include"NWPGameState.h"

// UE
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameStateBase.h"

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

class ANWPGameMode* UNWPGameplayStatics::GetNWPGameMode(const UObject* WorldContextObject)
{
	AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(WorldContextObject);
	check(GameModeBase);

	ANWPGameMode* CastGameModeBase = Cast<ANWPGameMode>(GameModeBase);
	check(CastGameModeBase);

	return CastGameModeBase;
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

class ANWPGameState* UNWPGameplayStatics::GetNWPGameState(const UObject* WorldContextObject)
{
	AGameStateBase* GameStateBase = UGameplayStatics::GetGameState(WorldContextObject);
	check(GameStateBase);

	ANWPGameState* CastGameStateBase = Cast<ANWPGameState>(GameStateBase);
	check(CastGameStateBase);

	return CastGameStateBase;
}
