// Copyright 2019 Neuron Station. All Rights Reserved.

// Class
#include "NWPWeaponTargetComponent.h"

// NWP
#include "NWPGameplayStatics.h"
#include "NWPGameState.h"
#include "NWPObjectRegistryComponent.h"
#include "NWPObjectRegistry.h"

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void UNWPWeaponTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	UNWPObjectRegistryComponent* ObjectRegistryComponent = UNWPGameplayStatics::GetNWPGameState(this)->GetObjectRegistryComponent();
	check(ObjectRegistryComponent && ObjectRegistryComponent->GetObjectRegistry());
	
	ObjectRegistryComponent->GetObjectRegistry()->RegisterObject<UNWPWeaponTargetComponent>(this);
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void UNWPWeaponTargetComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UNWPObjectRegistryComponent* ObjectRegistryComponent = UNWPGameplayStatics::GetNWPGameState(this)->GetObjectRegistryComponent();
	check(ObjectRegistryComponent && ObjectRegistryComponent->GetObjectRegistry());

	ObjectRegistryComponent->GetObjectRegistry()->UnregisterObject<UNWPWeaponTargetComponent>(this);

	Super::EndPlay(EndPlayReason);
}
