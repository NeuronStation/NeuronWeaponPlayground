// Copyright 2019 Neuron Station. All Rights Reserved.

// Class
#include "NWPGameplayStatics.h"

// NWP
#include"NeuronTestGameMode.h"

// UE
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

class ANeuronTestGameMode* UNWPGameplayStatics::GetNWPGameMode(const UObject* WorldContextObject)
{
	AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(WorldContextObject);
	check(GameModeBase);

	ANeuronTestGameMode* CastGameModeBase = Cast<ANeuronTestGameMode>(GameModeBase);
	check(CastGameModeBase);

	return CastGameModeBase;
}
