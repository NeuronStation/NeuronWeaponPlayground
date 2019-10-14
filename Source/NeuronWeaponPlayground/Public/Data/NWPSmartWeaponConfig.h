// Copyright 2019 Neuron Station. All Rights Reserved.

#pragma once


// Generated
#include "CoreMinimal.h"
#include "Data/NWPWeaponConfig.h"
#include "NWPSmartWeaponConfig.generated.h"

/**
 * Configuration for the smart weapon.
 */
UCLASS()
class NEURONWEAPONPLAYGROUND_API UNWPSmartWeaponConfig : public UNWPWeaponConfig
{
	GENERATED_BODY()

// Constructors
public:

	UNWPSmartWeaponConfig(const class FObjectInitializer& ObjectInitializer);

// Member functions
public:

	////////////////////////////////////////////////////////////////
	// Accesors

	// Returns the horizontal target area
	FORCEINLINE int32 GetHorizontalTargetArea() const { return HorizontalTargetArea; }

	// Returns the vertical target area
	FORCEINLINE int32 GetVerticalTargetArea() const { return VerticalTargetArea; }

	// Returns the larget lock size
	FORCEINLINE int32 GetTargetLockSize() const { return TargetLockSize; }

	// Returns the update projectiles delta time
	FORCEINLINE float GetUpdateProjectileDeltaTime() const { return UpdateProjectileDeltaTime; }

	// Returns the velocity used to orient the projectile to the target
	FORCEINLINE int32 GetOrientProjectileToTargetVelocity() const { return OrientProjectileToTargetVelocity; }

	// Returns the velocity used to orient the projectile to the point used to avoid the obstacle
	FORCEINLINE int32 GetOrientProjectileToAvoidObstacleVelocity() const { return OrientProjectileToAvoidObstacleVelocity; }

	// Returns the distance that the projectile is going to used in order to check if an obstacle is in front of it
	FORCEINLINE float GetAvoidObstacleProjectileDistance() const { return AvoidObstacleProjectileDistance; }

	// Returns the distance added to the left / right avoid points
	FORCEINLINE int32 GetAvoidObstacleHorizontalOffset() const { return AvoidObstacleHorizontalOffset; }

	// Returns the distance added to the up avoid point
	FORCEINLINE int32 GetAvoidObstacleVerticalOffset() const { return AvoidObstacleVerticalOffset; }

// Member variables
protected:

	///////////////////////////////////////////////////////////////////////////
	// Configuration

	// Specifies the horizontal target area
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smart Weapon Configuration")
	int32 HorizontalTargetArea;

	// Specifies the vertical target area
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smart Weapon Configuration")
	int32 VerticalTargetArea;

	// The size of the target lock that is going to be drawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smart Weapon Configuration")
	int32 TargetLockSize;

	// Time between the projectile update
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smart Weapon Configuration")
	float UpdateProjectileDeltaTime;

	// Velocity used to orient the projectile to the target
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smart Weapon Configuration")
	float OrientProjectileToTargetVelocity;

	// Velocity used to orient the projectile to the point used to avoid the obstacle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smart Weapon Configuration")
	float OrientProjectileToAvoidObstacleVelocity;

	// The distance that the projectile is going to used in order to check if an obstacle is in front of it
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smart Weapon Configuration")
	float AvoidObstacleProjectileDistance;

	// Distance added to the left / right avoid points
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smart Weapon Configuration")
	float AvoidObstacleHorizontalOffset;

	// Distance added to the up avoid point
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smart Weapon Configuration")
	float AvoidObstacleVerticalOffset;
};
