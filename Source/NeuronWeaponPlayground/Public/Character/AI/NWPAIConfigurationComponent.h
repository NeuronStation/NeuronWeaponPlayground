// Copyright 2019 Neuron Station. All Rights Reserved.

#pragma once

// Generated
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NWPAIConfigurationComponent.generated.h"

/**
 * AI configuration component that contains data relevant for the ai.
 */
UCLASS( ClassGroup=(AI), meta=(BlueprintSpawnableComponent) )
class NEURONWEAPONPLAYGROUND_API UNWPAIConfigurationComponent : public UActorComponent
{
	GENERATED_BODY()

// Constructors
public:

	UNWPAIConfigurationComponent(const class FObjectInitializer& ObjectInitializer);

// Member functions
public:

	// Returns the behavior tree configured for the ai
	FORCEINLINE class UBehaviorTree* GetConfiguredBehaviorTree() const { return ConfiguredBehaviorTree; }

	// Returns if the ai is a follower
	FORCEINLINE bool IsFollower() const { return bIsFollower; }

// Member variables
protected:

	// Behavior tree configured for the ai
	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* ConfiguredBehaviorTree;

	// Indicates if the ai is a follower
	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = "true"))
	bool bIsFollower;
};
