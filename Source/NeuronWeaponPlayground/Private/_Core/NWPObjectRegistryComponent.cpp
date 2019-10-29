// Copyright 2019 Neuron Station. All Rights Reserved.

// Class
#include "NWPObjectRegistryComponent.h"

// NWP
#include "NWPObjectRegistry.h"

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

UNWPObjectRegistryComponent::UNWPObjectRegistryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	ObjectRegistry = CreateDefaultSubobject<UNWPObjectRegistry>(TEXT("ObjectRegistry"));
	check(ObjectRegistry);
}
