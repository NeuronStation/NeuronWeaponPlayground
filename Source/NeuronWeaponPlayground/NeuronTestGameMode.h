// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NeuronTestGameMode.generated.h"

UCLASS(minimalapi)
class ANeuronTestGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:
	ANeuronTestGameMode();

	class UNWPObjectRegistry* ObjectRegistry;
};



