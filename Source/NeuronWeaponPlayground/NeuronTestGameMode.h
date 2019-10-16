// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

// NWP
#include "NWPObjectRegistryInterface.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NeuronTestGameMode.generated.h"

UCLASS(minimalapi)
class ANeuronTestGameMode : public AGameModeBase, public INWPObjectRegistryInterface
{
	GENERATED_BODY()

public:

	ANeuronTestGameMode();

public:

	virtual class UNWPObjectRegistryComponent* GetObjectRegistryComponent() override;

protected:

	class UNWPObjectRegistryComponent* ObjectRegistryComponent;

};



