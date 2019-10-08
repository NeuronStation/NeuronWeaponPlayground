// Copyright 2019 Neuron Station. All Rights Reserved.

#pragma once

// NWP
#include "NeuronWeaponPlayground.h"

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NWPAnimInstanceCharacter.generated.h"

/**
 * Animation controller of the character. Reads Gameplay variables that are going to be used by the AnimGraph.
 * Plays animation montages on the character
 */
UCLASS()
class NEURONWEAPONPLAYGROUND_API UNWPAnimInstanceCharacter : public UAnimInstance
{
	GENERATED_BODY()

// Constructors
public:

	UNWPAnimInstanceCharacter(const class FObjectInitializer& ObjectInitializer);

// Member functions
public:

	/// UAnimInstance interface begin
	// Initialization
	virtual void NativeInitializeAnimation() override;

	// Animation tick
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	/// UAnimInstance interface end

	// Plays a character anim montage
	void PlayActionMontage(class UAnimMontage* MontageToPlay, float _PlayRate = 1.0f);

// Member variables
protected:

	// Reference to the owning character 
	UPROPERTY(Transient, SkipSerialization)
	class ANeuronTestCharacter* OwnerCharacter;

	// Montage currently playing
	UPROPERTY(Transient, SkipSerialization)
	class UAnimMontage* CurrentAnimationMontage;

	// Indicates that the weapon is busy
	UPROPERTY(Transient, EditAnywhere, Category = Transient, BlueprintReadOnly)
	bool bIsBusy;

	// The current weapon state
	UPROPERTY(Transient, EditAnywhere, Category = Transient, BlueprintReadOnly)
	ENWPWeaponState CurrentWeaponState;
};
