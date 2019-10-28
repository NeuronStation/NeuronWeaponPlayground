// Copyright 2019 Neuron Station. All Rights Reserved.

// Class
#include "NWPGameState.h"

// NWP
#include "NWPObjectRegistryComponent.h"

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

ANWPGameState::ANWPGameState(const class FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	ObjectRegistryComponent(nullptr)
{
	ObjectRegistryComponent = CreateDefaultSubobject<UNWPObjectRegistryComponent>(TEXT("ObjectRegistryComponent"));
}
