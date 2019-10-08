// Copyright 2019 Neuron Station. All Rights Reserved.

#pragma once

// NWP
#include "NWPSmartWeaponConfig.h"

#include "CoreMinimal.h"
#include "Weapons/NWPWeapon.h"
#include "NWPSmartWeapon.generated.h"

// Struct that contains information of a smart controlled projectile
USTRUCT()
struct FNWPSmartProjectileData
{
	GENERATED_USTRUCT_BODY()

// Constructors
public:

	FNWPSmartProjectileData()
	{
		TargetActor = nullptr;
		TargetObstacle = nullptr;
		AvoidObstaclePoint = FVector::ZeroVector;
		CurrentState = ENWPSmartProjectileState::OrientatingToTarget;
	}

	FNWPSmartProjectileData(const AActor* _TargetActor)
	{
		TargetActor = _TargetActor;
		TargetObstacle = nullptr;
		AvoidObstaclePoint = FVector::ZeroVector;
		CurrentState = ENWPSmartProjectileState::OrientatingToTarget;
	}

// Member functions
public:

	// Get the target actor
	const AActor* GetTargetActor() const { return TargetActor; }

	// Get the target obstacle
	const AActor* GetTargetObstacle() const { return TargetObstacle; }

	// Set the target obstacle
	void SetTargetObstacle(const AActor* _TargetObstacle) { TargetObstacle = _TargetObstacle; }

	// Get the point to avoid the obstacle
	FVector GetAvoidObstaclePoint() const { return AvoidObstaclePoint; }

	// Set the point to avoid the obstacle
	void SetAvoidObstaclePoint(FVector _AvoidObstaclePoint) { AvoidObstaclePoint = _AvoidObstaclePoint; }

	// Returns if the projectile is orientating to avoid obstacles
	bool IsOrientatingToAvoidObstacles() const { return CurrentState == ENWPSmartProjectileState::OrientatingToAvoidObstacle; }

	// Returns if the projectile is orientating to target
	bool IsOrientatingToTarget() const { return CurrentState == ENWPSmartProjectileState::OrientatingToTarget; }

	// Returns if the projectile has hit with something
	bool HasHitWithSomthing() const { return CurrentState == ENWPSmartProjectileState::HitWithSomething; }

	// Get the projectile current state
	ENWPSmartProjectileState GetCurrentState() const { return CurrentState; }

	// Set the projectile current state
	void SetCurrentState(ENWPSmartProjectileState _CurrentState) { CurrentState = _CurrentState; }

// Member variables
protected:

	// The target actor of the projectile 
	UPROPERTY(Transient, SkipSerialization)
	const AActor* TargetActor;

	// The target obstacle of the projectile 
	UPROPERTY(Transient, SkipSerialization)
	const AActor* TargetObstacle;

	// Point used to avoid the obstacle
	UPROPERTY(Transient, SkipSerialization)
	FVector AvoidObstaclePoint;

	// The current state of the projectile
	UPROPERTY(Transient, SkipSerialization)
	ENWPSmartProjectileState CurrentState;
};

/**
 * Weapon that can launch projectiles that can follow a target and avoid some obstacles. Can be configured using UNWPSmartWeaponConfig
 */
UCLASS()
class NEURONWEAPONPLAYGROUND_API ANWPSmartWeapon : public ANWPWeapon
{
	GENERATED_BODY()

// Constructors
public:

	ANWPSmartWeapon(const class FObjectInitializer& ObjectInitializer);

// Member functions
public:

	/// AActor interface begin
	// Function called every frame on this Actor. 
	virtual void Tick(float DeltaSeconds) override;
	/// AActor interface end

	///////////////////////////////////////////////////////////////////////////
	// Accessors

	// Returns the smart weapon config
	FORCEINLINE const class UNWPSmartWeaponConfig* GetSmartWeaponConfig() const { return Cast<UNWPSmartWeaponConfig>(CurrentWeaponConfig); }

	// Returns the current targets
	FORCEINLINE const TArray<AActor*> GetCurrentTargets() const { return CurrentTargets; }

	// Returns the target area begin position
	FORCEINLINE FVector2D GetTargetAreaBeginPosition() const { return TargetAreaBeginPosition; }

	// Returns the target area begin
	FORCEINLINE FVector2D GetTargetArea() const { return TargetArea; }

protected:

	/// ANWPWeapon interface begin
	// Callback executed when the weapon loads
	virtual void OnWeaponLoaded() override;
	/// ANWPWeapon interface end

	///////////////////////////////////////////////////////////////////////////
	// Target

	// Returns if at least there is one target to shoot
	bool HasTargetToShoot();

	// Returns the target to shoot
	const AActor* GetTargetToShoot();

	// Calculate the viewport target positions according to the current
	void CalculateViewportTargetPositions();

	// Update the targets
	void UpdateTargets();

	// Evaluate if a target is inside the target area
	bool IsActorInsideTargetArea(class AActor* ActorToEvaluate);

	///////////////////////////////////////////////////////////////////////////
	// Projectile

	// Spawns the projectile
	virtual void SpawProjectile() override;

	// Get the avoid obstacle point according to the obstacle
	FVector GetAvoidObstaclePoint(class ANWPProjectile* _ProjectileToProcess, class AActor* TargetObstacle);

	// Updates the smart projectiles
	void UpdateSmartProjectiles(float DeltaTime);

	// Updates a smart projectile
	void UpdateSmartProjectile(class ANWPProjectile* _ProjectileToProcess, float DeltaTime);

	// Callback executed after the projectile velocity has been computed
	virtual void OnProjectileVelocityComputed(class ANWPProjectile* _ProjectileToProcess, FVector& _ComputedVelocity, float DeltaTime) override;

	// Callback executed when the projectile has hit something
	virtual void OnProjectileHit(class ANWPProjectile* _ProjectileToProcess, class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
		FVector NormalImpulse, const FHitResult& Hit);

	// Callback executed before the projectile is destroyed
	virtual void OnProjectileIsGoingToBeDestroyed(class ANWPProjectile* _ProjectileToProcess) override;

// Member variables
protected:

	// Targets inside the target area
	UPROPERTY(Transient, SkipSerialization)
	TArray<AActor*> CurrentTargets;

	// The target area begin position
	UPROPERTY(Transient, SkipSerialization)
	FVector2D TargetAreaBeginPosition;

	// The target area. The X is the horizontal size. The Y is the vertical size
	UPROPERTY(Transient, SkipSerialization)
	FVector2D TargetArea;

	// Map that contains information about the smart projectiles
	UPROPERTY(Transient, SkipSerialization)
	TMap<ANWPProjectile*, FNWPSmartProjectileData> SmartProjectiles;

	// Time used to evaluate if the projectiles have to be updated
	UPROPERTY(Transient, SkipSerialization)
	float CurrentUpdateProjectilesTime;

};
