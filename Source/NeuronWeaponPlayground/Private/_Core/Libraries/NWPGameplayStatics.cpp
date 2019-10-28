// Copyright 2019 Neuron Station. All Rights Reserved.

// Class
#include "NWPGameplayStatics.h"

// NWP
#include"NWPGameMode.h"

// UE
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"

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
