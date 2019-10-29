// Copyright 2019 Neuron Station. All Rights Reserved.

#pragma once

// Generated
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NWPGameplayStatics.generated.h"

/**
 * Static class with useful gameplay utility functions that can be called from both Blueprint and C++. NWP abstraction layer.
 */
UCLASS()
class NEURONWEAPONPLAYGROUND_API UNWPGameplayStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
// Member functions
public:

	// NWP version of UGameplayStatics::GetGameMode. Returns always a valid cast ANWPGameMode
	UFUNCTION(BlueprintPure, Category = "Game NWP", meta = (WorldContext = "WorldContextObject"))
	static class ANWPGameMode* GetNWPGameMode(const UObject* WorldContextObject);


	// NWP version of UGameplayStatics::GetGameState. Returns always a valid cast ANWPGameState
	UFUNCTION(BlueprintPure, Category = "Game NWP", meta = (WorldContext = "WorldContextObject"))
	static class ANWPGameState* GetNWPGameState(const UObject* WorldContextObject);
};
