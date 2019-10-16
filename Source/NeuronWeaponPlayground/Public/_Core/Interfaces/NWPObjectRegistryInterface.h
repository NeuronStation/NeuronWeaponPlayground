// Copyright 2019 Neuron Station. All Rights Reserved.

#pragma once

// Generated
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NWPObjectRegistryInterface.generated.h"

// This class does not need to be modified
UINTERFACE(MinimalAPI)
class UNWPObjectRegistryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface that forces the class that implements it to return a UNWPObjectRegistryComponent.
 */
class NEURONWEAPONPLAYGROUND_API INWPObjectRegistryInterface
{
	GENERATED_BODY()

// Member functions
public:

	// Returns the component which contains the object registry
	virtual class UNWPObjectRegistryComponent* GetObjectRegistryComponent() = 0;
};
