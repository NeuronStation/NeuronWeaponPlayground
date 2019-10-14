// Copyright 2019 Neuron Station. All Rights Reserved.

#pragma once

// Generated
#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NWPProjectileMovementComponent.generated.h"

/**
 * Component that controls the movement of a projectile. Tells the weapon when some calculations related to the movement are perfomed.
 */
UCLASS()
class NEURONWEAPONPLAYGROUND_API UNWPProjectileMovementComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()

// Member functions
public:

	/// UProjectileMovementComponent interface begin
	// Given an initial velocity and a time step, compute a new velocity. Default implementation applies the result of ComputeAcceleration() to velocity
	virtual FVector ComputeVelocity(FVector InitialVelocity, float DeltaTime) const;
	/// UProjectileMovementComponent interface end
};
