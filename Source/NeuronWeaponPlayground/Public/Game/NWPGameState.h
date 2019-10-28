// Copyright 2019 Neuron Station. All Rights Reserved.

#pragma once

// Generated
#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NWPGameState.generated.h"

/**
 * NWP abstraction level GameState.
 */
UCLASS()
class NEURONWEAPONPLAYGROUND_API ANWPGameState : public AGameStateBase
{
	GENERATED_BODY()

// Constructors
public:

	ANWPGameState(const class FObjectInitializer& ObjectInitializer);

// Member functions
public:

	// Returns the object registry component
	FORCEINLINE class UNWPObjectRegistryComponent* GetObjectRegistryComponent() const { return ObjectRegistryComponent; }

// Member variables
protected:

	// Component which contains the object registry
	class UNWPObjectRegistryComponent* ObjectRegistryComponent;
};
