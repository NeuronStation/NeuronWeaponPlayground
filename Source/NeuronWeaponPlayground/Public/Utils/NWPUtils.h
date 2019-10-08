// // Plugin that implements a simple and scalable weapon system developed by Neuron Station

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NWPUtils.generated.h"

/**
 * A set of useful functions
 */
UCLASS()
class NEURONWEAPONPLAYGROUND_API UNWPUtils : public UObject
{
	GENERATED_BODY()

// Member functions
public:

	// This function retrieves the name for a given enum value.
	template <typename T>
	static FString GetEnumName(const TCHAR* EnumName, const T EnumValue)
	{
		static_assert(TIsEnum<T>::Value, "Should only call this with enum types");

		// Find the enum and return the value
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, EnumName, true);
		return EnumPtr ? EnumPtr->GetNameStringByValue(static_cast<int64>(EnumValue)) : FString(TEXT("Invalid"));
	}
};
