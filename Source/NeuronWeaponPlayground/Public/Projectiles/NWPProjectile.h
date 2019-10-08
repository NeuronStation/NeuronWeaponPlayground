// Copyright 2019 Neuron Station. All Rights Reserved.

#pragma once

// NWP
#include "NWPProjectileMovementComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NWPProjectile.generated.h"

/**
 * Projectile that can be launched by a weapon
 * TODO: [NWP-REVIEW] Consider if it is necessary to have a UNWPProjectileConfig
 */
UCLASS()
class NEURONWEAPONPLAYGROUND_API ANWPProjectile : public AActor
{
	GENERATED_BODY()

// Friend class
friend class UNWPProjectileMovementComponent;

// Constructors
public:

	ANWPProjectile(const class FObjectInitializer& ObjectInitializer);

// Member functions
public:

	/// AActor interface begin
	// Overridable native event for when play begins for this actor.
	virtual void BeginPlay() override;
	/// AActor interface end

	////////////////////////////////////////////////////////////////
	// Accesors

	// Returns the projectile movement component
	FORCEINLINE class UNWPProjectileMovementComponent* GetNWPProjectileMovementComponent() const  { return ProjectileMovement; };

	////////////////////////////////////////////////////////////////
	// Owner

	// This function that sets the owner character for this weapon
	void SetOwnerWeapon(class ANWPWeapon* _NewOwnerWeapon);

protected:

	// Called when projectile hits something
	UFUNCTION()
	void OnHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Callback executed after the projectile velocity has been computed
	virtual void OnProjectileVelocityComputed(FVector& _ComputedVelocity, float DeltaTime);

// Member variables
protected:

	///////////////////////////////////////////////////////////////////////////
	// Configuration

	// Effect spawned where the projectile has impact
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Configuration")
	UParticleSystem* ImpactEffect;

	// Effect spawned on the projectile when spawned
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Configuration")
	UParticleSystem* TracerEffect;

	// Multiplicative factor applied to the impulse acting on the actor with whom the projectile collides
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Configuration")
	float ImpulseStrenghtFactor;

	////////////////////////////////////////////////////////////////
	// Components

	// Sphere collision component 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollisionComp;

	// Projectile movement component 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	class UNWPProjectileMovementComponent* ProjectileMovement;

	// Particle tracer currently spawned
	UPROPERTY(Transient, SkipSerialization)
	class UParticleSystemComponent* TracerComponent;

	////////////////////////////////////////////////////////////////
	// State

	// Reference to the owning weapon
	UPROPERTY(Transient, SkipSerialization)
	class ANWPWeapon* OwnerWeapon;
};
