// Copyright 2019 Neuron Station. All Rights Reserved.

// Class
#include "NWPProjectileMovementComponent.h"

// NWP
#include "NWPProjectile.h"

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

FVector UNWPProjectileMovementComponent::ComputeVelocity(FVector InitialVelocity, float DeltaTime) const
{
	FVector ComputedVelocity = Super::ComputeVelocity(InitialVelocity, DeltaTime);

	// Tell the projectile that the velocity has been computed
	ANWPProjectile* Projectile = Cast<ANWPProjectile>(GetOwner());
	if (Projectile)
	{
		Projectile->OnProjectileVelocityComputed(ComputedVelocity, DeltaTime);
	}

	return ComputedVelocity;
}
