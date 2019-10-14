// Copyright 2019 Neuron Station. All Rights Reserved.

// Class
#include "NWPProjectile.h"

// UE
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

// NWP
#include "NWPWeapon.h"
#include "NWPProjectileMovementComponent.h"

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

ANWPProjectile::ANWPProjectile(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ANWPProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UNWPProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	// Initialize some values
	TracerComponent = nullptr;
	ImpulseStrenghtFactor = 10.0f;
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Try to spawn the tracer effect
	if (TracerEffect)
	{
		TracerComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, GetActorTransform());

		if (TracerComponent)
		{
			TracerComponent->AttachToComponent(CollisionComp, FAttachmentTransformRules::SnapToTargetIncludingScale);
			TracerComponent->SetAbsolute(false, false, false);
		}
	}
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPProjectile::SetOwnerWeapon(class ANWPWeapon* _NewOwnerWeapon)
{
	OwnerWeapon = _NewOwnerWeapon;
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPProjectile::OnHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogNWP, Log, TEXT("Projectile %s has hit: HitActor: %s HitComponent: %s "), *this->GetName(), *OtherActor->GetName(), *OtherComp->GetName());

	// Tell the weapon that the projectile has hit something
	if (OwnerWeapon)
	{
		OwnerWeapon->OnProjectileHit(this, HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
	}

	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * ImpulseStrenghtFactor, GetActorLocation());
	}

	// Tell the weapon owner that the projectile is going to be destroyed
	if (OwnerWeapon)
	{
		OwnerWeapon->OnProjectileIsGoingToBeDestroyed(this);
	}

	// Deactivate tracer component
	if (TracerComponent)
	{
		TracerComponent->DestroyComponent(true);
	}

	// Try to spawn the hit effect
	if (ImpactEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint, Hit.Normal.Rotation());
	}

	// Destroy the projectile
	Destroy();
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPProjectile::OnProjectileVelocityComputed(FVector& _ComputedVelocity, float DeltaTime)
{
	// Tell the weapon that the velocity has been computed
	if (OwnerWeapon)
	{
		OwnerWeapon->OnProjectileVelocityComputed(this, _ComputedVelocity, DeltaTime);
	}
}
