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

	// Returns if the ai is compatible with a given behavior tree
	FORCEINLINE bool IsCharacterBehaviorTreeCompatible(class UBehaviorTree* CharacterBehaviorTree) const
	{
		return CompatibleCharacterBehaviorTree.Contains(CharacterBehaviorTree);
	}

	// Returns if the ai is a follower
	FORCEINLINE bool IsFollower() const { return bIsFollower; }

// Member variables
protected:

	// Indicates the behavior trees compatible with the ai
	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TSet<class UBehaviorTree*> CompatibleCharacterBehaviorTree;

	// Indicates if the ai is a follower
	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = "true"))
	bool bIsFollower;
};
