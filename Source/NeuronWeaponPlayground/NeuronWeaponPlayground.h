// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// Collision projectiles
#define COLLISION_WEAPON			ECC_GameTraceChannel1

// Console variables
static TAutoConsoleVariable<int32> CVarbDebugWeapon(
	TEXT("NWP.bDebugWeapon"),
	0,
	TEXT("Shows the debug information of the weapon.\n")
	TEXT("0: Disables display of weapon debug information. \n")
	TEXT("1: Enables display of weapon debug information. \n"),
	ECVF_Default);

// Log category
DECLARE_LOG_CATEGORY_EXTERN(LogNWP, Log, All);

// Enum for the states of the weapon
UENUM(BlueprintType)
enum class ENWPWeaponState : uint8
{
	Invalid,
	None,
	Shooting,
	Reloading,
};

// Enum for the types of cadence
UENUM(BlueprintType)
enum class ENWPWeaponCadenceType : uint8
{
	Automatic,
	SemiAutomatic,
	COUNT,
};

// Enum for the cadence configurations
UENUM(BlueprintType)
enum class ENWPWeaponCadenceConfig : uint8
{
	Automatic,
	SemiAutomatic,
	Both,
};

// Enum for the states of the smart projectiles
UENUM(BlueprintType)
enum class ENWPSmartProjectileState : uint8
{
	OrientatingToAvoidObstacle,
	OrientatingToTarget,
	HitWithSomething,
};