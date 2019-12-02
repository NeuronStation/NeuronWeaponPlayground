// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

// Generated
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NWPGameMode.generated.h"

/**
 * NWP abstraction level GameMode.
 */
UCLASS(minimalapi)
class ANWPGameMode : public AGameModeBase
{
	GENERATED_BODY()

// Constructors
public:

	ANWPGameMode(const class FObjectInitializer& ObjectInitializer);

// Member functions
protected:

	/// AActor interface begin
	// Overridable native event for when play begins for this actor
	virtual void BeginPlay() override;
	/// AActor interface end

};
