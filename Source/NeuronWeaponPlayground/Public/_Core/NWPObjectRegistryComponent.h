// Copyright 2019 Neuron Station. All Rights Reserved.

#pragma once

// Generated
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NWPObjectRegistryComponent.generated.h"

/**
 * Component that contains an ObjectRegistry.
 */
UCLASS( ClassGroup=(Core), meta=(BlueprintSpawnableComponent) )
class NEURONWEAPONPLAYGROUND_API UNWPObjectRegistryComponent : public UActorComponent
{
	GENERATED_BODY()

// Constructor
public:	
	
	UNWPObjectRegistryComponent();

// Member functions
public:

	// Returns the object registry associated to the component
	FORCEINLINE class UNWPObjectRegistry* GetObjectRegistry() { return ObjectRegistry; }

// Member variables
protected:

	// Registry of objects associated to the component
	class UNWPObjectRegistry* ObjectRegistry;
		
};
