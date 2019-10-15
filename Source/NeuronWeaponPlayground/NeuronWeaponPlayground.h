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

// ------------------------------------- Extended Log -------------------------------------

// For more information: https://wiki.unrealengine.com/Logs,_Printing_Class_Name,_Function_Name,_Line_Number_of_your_Calling_Code!

/*
	Joy String
		Current Class, File, and Line Number!
			by Rama

	PreProcessor commands to get
		a. Class name
		b. Function Name
		c. Line number
		d. Function Signature (including parameters)

	Gives you a UE4 FString anywhere in your code that these macros are used!

	Ex:
		You can use JOYSTR_CUR_CLASS anywhere to get a UE4 FString back telling you
		what the current class is where you called this macro!

	Ex:
		This macro prints the class and line along with the message of your choosing!
		VSCREENMSG("Have fun today!");
	<3  Rama
*/
#pragma once

//Current Class Name + Function Name where this is called!
#define JOYSTR_CUR_CLASS_FUNC (FString(__FUNCTION__))

//Current Class where this is called!
#define JOYSTR_CUR_CLASS (FString(__FUNCTION__).Left(FString(__FUNCTION__).Find(TEXT(":"))) )

//Current Function Name where this is called!
#define JOYSTR_CUR_FUNC (FString(__FUNCTION__).Right(FString(__FUNCTION__).Len() - FString(__FUNCTION__).Find(TEXT("::")) - 2 ))

//Current Line Number in the code where this is called!
#define JOYSTR_CUR_LINE  (FString::FromInt(__LINE__))

//Current Class and Line Number where this is called!
#define JOYSTR_CUR_CLASS_LINE (JOYSTR_CUR_CLASS + "(" + JOYSTR_CUR_LINE + ")")

//Current Function Signature where this is called!
#define JOYSTR_CUR_FUNCSIG (FString(__FUNCSIG__))


//Victory Screen Message
// 	Gives you the Class name and exact line number where you print a message to yourself!
#define VSCREENMSG(Param1) (GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *(JOYSTR_CUR_CLASS_LINE + ": " + Param1)) )

#define VSCREENMSG2(Param1,Param2) (GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *(JOYSTR_CUR_CLASS_LINE + ": " + Param1 + " " + Param2)) )

#define VSCREENMSGF(Param1,Param2) (GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *(JOYSTR_CUR_CLASS_LINE + ": " + Param1 + " " + FString::SanitizeFloat(Param2))) )

//UE LOG!
#define V_LOG(LogCat, LogVerbosity, Param1) 		UE_LOG(LogCat,LogVerbosity,TEXT("%s: %s"), *JOYSTR_CUR_CLASS_LINE, *FString(Param1))

#define V_LOG2(LogCat, LogVerbosity, Param1,Param2) 	UE_LOG(LogCat,LogVerbosity,TEXT("%s: %s %s"), *JOYSTR_CUR_CLASS_LINE, *FString(Param1),*FString(Param2))

#define V_LOGF(LogCat, LogVerbosity, Param1,Param2) 	UE_LOG(LogCat,LogVerbosity,TEXT("%s: %s %f"), *JOYSTR_CUR_CLASS_LINE, *FString(Param1),Param2)

#define V_LOGM(LogCat, LogVerbosity, FormatString , ...) UE_LOG(LogCat,LogVerbosity,TEXT("%s: %s"), 	*JOYSTR_CUR_CLASS_LINE, *FString::Printf(TEXT(FormatString), ##__VA_ARGS__ ) )

// ------------------------------------- /Extended Log -------------------------------------

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