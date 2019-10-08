// // Plugin that implements a simple and scalable weapon system developed by Neuron Station

#include "NWPWeaponConfig.h"

#include "NeuronWeaponPlayground.h"

UNWPWeaponConfig::UNWPWeaponConfig(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CurrentLoadFinishedDelegate = nullptr;
	bIsLoading = false;
	CachedWeaponMesh = nullptr;
	CachedMuzzleEffect = nullptr;
	CachedShootSound = nullptr;
	CachedShootingMontage = nullptr;
	InitialAmmo = 30;
	MaximumAmmo = 100;
	AmmoPerMagazine = 30;
	MagazineReloadTime = 0.5;
	ShootDistance = 10000.0f;
	bUseProjectileAsAmmo = false;
	bUseEyesAsShootOrigin = true;
	MuzzleSocketName = TEXT("");
	EyesOffsetLocation = FVector::ZeroVector;
	EyesOffsetRotation = FRotator::ZeroRotator;
	MuzzleBoneOffsetLocation = FVector::ZeroVector;
	MuzzleBoneOffsetRotation = FRotator::ZeroRotator;
}

ENWPWeaponCadenceType UNWPWeaponConfig::GetCadenceType() const
{
	// Check if the weapon is automatic or semi automatic
	if (IsAutomatic())
	{
		return ENWPWeaponCadenceType::Automatic;
	}
	else if(IsSemiAutomatic())
	{
		return ENWPWeaponCadenceType::SemiAutomatic;
	}

	return ENWPWeaponCadenceType::COUNT;
}

void UNWPWeaponConfig::LoadWeaponConfig(bool bSyncLoad, const FNWPOnWaponConfigLoaded& _Callback)
{
	// Check if there is a load in progress
	if (bIsLoading)
	{
		return;
	}

	// Bind callback if specified
	if (_Callback.IsBound())
	{
		// Bind the function to the delegate
		CurrentLoadFinishedDelegate = _Callback;
	}

	// Mark the loading flag
	bIsLoading = true;

	// Check if the load has to be synchronous
	if (bSyncLoad)
	{
		// TODO: [NWP-REVIEW] This cache references load system does not scale well.

		// Load weapon mesh
		if (!WeaponMesh.IsNull())
		{
			CachedWeaponMesh = WeaponMesh.LoadSynchronous();
		}

		// Load effects
		if (!MuzzleEffect.IsNull())
		{
			CachedMuzzleEffect = MuzzleEffect.LoadSynchronous();
		}

		// Load sounds
		if (!ShootSound.IsNull())
		{
			CachedShootSound = ShootSound.LoadSynchronous();
		}

		// Load montages
		if (!ShootMontage.IsNull())
		{
			CachedShootingMontage = ShootMontage.LoadSynchronous();
		}

		// Finish the weapon load
		FinishWeaponConfigLoad();
	}
	else
	{
		// TODO: [NWP-REVIEW] Implement weapon config async load
		UE_LOG(LogNWP, Warning, TEXT("UNWPWeaponConfig::LoadWeaponConfig: WeaponConfig async load yet unimplemented"));
	}
}

void UNWPWeaponConfig::ReleaseWeaponConfig()
{
	// Release the hard references
	CachedWeaponMesh = nullptr;
	CachedShootingMontage = nullptr;
}

void UNWPWeaponConfig::FinishWeaponConfigLoad()
{
	// Return if not loading
	if (!bIsLoading)
	{
		return;
	}

	// Cache the loaded assets
	if (WeaponMesh)
	{
		CachedWeaponMesh = WeaponMesh.Get();
	}

	// Reset load mark
	bIsLoading = false;

	// Execute callback
	if (CurrentLoadFinishedDelegate.IsBound())
	{
		CurrentLoadFinishedDelegate.ExecuteIfBound();
		CurrentLoadFinishedDelegate = nullptr;
	}
}
