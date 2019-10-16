// Copyright 2019 Neuron Station. All Rights Reserved.

#pragma once

// NWP
#include "NWPObjectRegistryInterface.h"
#include "NWPObjectRegistryComponent.h"
#include "NWPObjectRegistry.h"

// UE
#include "Kismet/GameplayStatics.h"

// Generated
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NWPObjectRegistryLibrary.generated.h"

/**
 * Utility functions related to the Object Registry.
 */
UCLASS()
class NEURONWEAPONPLAYGROUND_API UNWPObjectRegistryLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

// Member functions
public:

	// Returns the first object registry found
	static UNWPObjectRegistry* GetFirstObjectRegistry(UObject* _WorldContext)
	{
		// TODO: [NWP-REVIEW] Check if this is the best way to obtain the actors that implements an interface
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsWithInterface(_WorldContext->GetWorld(), UNWPObjectRegistryInterface::StaticClass(), OutActors);

		if (OutActors.Num() == 0)
		{
			return nullptr;
		}

		return TryGetObjectRegistryFromActor(OutActors[0]);
	}

	// Register an object to all object registries
	template <typename T>
	static void RegisterObject(UObject* _ObjectToRegister)
	{
		// TODO: [NWP-REVIEW] Check if this is the best way to obtain the actors that implements an interface
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsWithInterface(_ObjectToRegister->GetWorld(), UNWPObjectRegistryInterface::StaticClass(), OutActors);

		// Obtain the object registry from the interface and register the object
		for (int32 Index = 0; Index < OutActors.Num(); ++Index)
		{
			UNWPObjectRegistry* ObjectRegistry = TryGetObjectRegistryFromActor(OutActors[Index]);
			check(ObjectRegistry);
			ObjectRegistry->RegisterObject<T>(_ObjectToRegister);
		}
	}

	// Unregister an object from all object registries
	template <typename T>
	static void UnregisterObject(UObject* _ObjectToRegister)
	{
		// TODO: [NWP-REVIEW] Check if this is the best way to obtain the actors that implements an interface
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsWithInterface(_ObjectToRegister->GetWorld(), UNWPObjectRegistryInterface::StaticClass(), OutActors);

		// Obtain the object registry from the interface and unregister the object
		for (int32 Index = 0; Index < OutActors.Num(); ++Index)
		{
			UNWPObjectRegistry* ObjectRegistry = TryGetObjectRegistryFromActor(OutActors[Index]);
			check(ObjectRegistry);
			ObjectRegistry->UnregisterObject<T>(_ObjectToRegister);
		}
	}

protected:

	// Obtains the object registry from an actor
	static UNWPObjectRegistry* TryGetObjectRegistryFromActor(AActor* _ObjectRegistryActor)
	{
		INWPObjectRegistryInterface* ObjectRegistryInterface = Cast<INWPObjectRegistryInterface>(_ObjectRegistryActor);

		// Check if the actor implements the interface
		if (!ObjectRegistryInterface)
		{
			return nullptr;
		}

		// Check if the object registry component returned by the interface is valid
		UNWPObjectRegistryComponent* ObjectRegistryComponent = ObjectRegistryInterface->GetObjectRegistryComponent();
		
		if (!ObjectRegistryComponent)
		{
			return nullptr;
		}

		return ObjectRegistryComponent->GetObjectRegistry();
	}
};
