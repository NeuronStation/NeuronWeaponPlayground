// Copyright 2019 Neuron Station. All Rights Reserved.

#pragma once

// Generated
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NWPWeaponTargetComponent.generated.h"

/**
 * Component which indicates that the attached actor is a weapon target.
 */
UCLASS( ClassGroup=(Weapon), meta=(BlueprintSpawnableComponent) )
class NEURONWEAPONPLAYGROUND_API UNWPWeaponTargetComponent : public UActorComponent
{
	GENERATED_BODY()

// Member functions
protected:

	/// UActorComponent interface begin
	// Called when the owning Actor begins play or when the component is created if the Actor has already begun play.
	virtual void BeginPlay() override;

	// Called from AActor::EndPlay only if bHasBegunPlay is true
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	/// UActorComponent interface end
		
};
