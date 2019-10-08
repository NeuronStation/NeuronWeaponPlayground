// // Plugin that implements a simple and scalable weapon system developed by Neuron Station

#include "NWPProjectileMovementComponent.h"

// NWP
#include "NWPProjectile.h"

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
