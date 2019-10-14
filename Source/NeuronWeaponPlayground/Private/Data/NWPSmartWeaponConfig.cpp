// Copyright 2019 Neuron Station. All Rights Reserved.

// Class
#include "NWPSmartWeaponConfig.h"

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

UNWPSmartWeaponConfig::UNWPSmartWeaponConfig(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	VerticalTargetArea = 200.0f;
	HorizontalTargetArea = 500.0f;
	TargetLockSize = 30;
	UpdateProjectileDeltaTime = 0.1;
	OrientProjectileToTargetVelocity = 1.0f;
	OrientProjectileToAvoidObstacleVelocity = 1.0f;
	AvoidObstacleProjectileDistance = 10000.0f;
	AvoidObstacleHorizontalOffset = 200.f;
	AvoidObstacleVerticalOffset = 125.0f;
}
