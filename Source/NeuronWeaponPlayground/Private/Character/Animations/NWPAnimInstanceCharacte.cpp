// Copyright 2019 Neuron Station. All Rights Reserved.

#include "NWPAnimInstanceCharacter.h"

// NWP
#include "NeuronTestCharacter.h"
#include "NWPWeapon.h"

UNWPAnimInstanceCharacter::UNWPAnimInstanceCharacter(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	OwnerCharacter = nullptr;
	CurrentAnimationMontage = nullptr;
	bIsBusy = false;
	CurrentWeaponState = ENWPWeaponState::Invalid;
}

void UNWPAnimInstanceCharacter::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// Cache the owning pawn for use in Tick
	OwnerCharacter = Cast<ANeuronTestCharacter>(TryGetPawnOwner());
}

void UNWPAnimInstanceCharacter::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	// Read some data from the owner
	if (OwnerCharacter)
	{
		if (OwnerCharacter->GetWeapon())
		{
			bIsBusy = OwnerCharacter->GetWeapon()->GetWeaponState() != ENWPWeaponState::None;
			CurrentWeaponState = OwnerCharacter->GetWeapon()->GetWeaponState();
		}
	}
}

void UNWPAnimInstanceCharacter::PlayActionMontage(class UAnimMontage* MontageToPlay, float _PlayRate)
{
	// Return if no montage to play
	if (!MontageToPlay)
	{
		return;
	}

	// Stop the montage if playing
	if (CurrentAnimationMontage)
	{
		Montage_Stop(CurrentAnimationMontage->BlendOut.GetBlendTime(), CurrentAnimationMontage);
	}

	// Play the new montage
	Montage_Play(MontageToPlay, _PlayRate);
}
