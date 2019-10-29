// Copyright 2019 Neuron Station. All Rights Reserved.

#pragma once

// Generated
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NWPUtils.generated.h"

/**
 * A set of useful functions.
 */
UCLASS()
class NEURONWEAPONPLAYGROUND_API UNWPUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

// Member functions
public:

	// This function retrieves the name for a given enum value
	template <typename T>
	static FString GetEnumName(const TCHAR* EnumName, const T EnumValue)
	{
		static_assert(TIsEnum<T>::Value, "Should only call this with enum types");

		// Find the enum and return the value
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, EnumName, true);
		return EnumPtr ? EnumPtr->GetNameStringByValue(static_cast<int64>(EnumValue)) : FString(TEXT("Invalid"));
	}

	// This function retrieves the atomic C++ class give a class
	static UClass* GetAtomicCppClass(UClass* Class)
	{
		// Loop the class/super class and check if it has been compiled from blueprint
		while (Class)
		{
			if (!Class->HasAnyClassFlags(CLASS_CompiledFromBlueprint))
			{
				return Class;
			}

			Class = Class->GetSuperClass();
		}

		checkNoEntry();
		return nullptr;
	}
};
