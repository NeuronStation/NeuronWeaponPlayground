// Copyright 2019 Neuron Station. All Rights Reserved.

#pragma once

// NWP
#include "NeuronWeaponPlayground.h"

// UE
#include "Engine/SkeletalMesh.h"
#include "Particles/ParticleSystem.h"

// Generated
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NWPWeaponConfig.generated.h"

// Delegates
DECLARE_DELEGATE(FNWPOnWeaponConfigLoadedDelegate)

/**
 * Configuration for a weapon.
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class NEURONWEAPONPLAYGROUND_API UNWPWeaponConfig : public UObject
{
	GENERATED_BODY()

// Constructors
public:

	UNWPWeaponConfig(const class FObjectInitializer& ObjectInitializer);
	
// Member functions
public:

	////////////////////////////////////////////////////////////////
	// Accesors

	// Returns the cached component mesh
	FORCEINLINE USkeletalMesh* GetWeaponMesh() const { return CachedWeaponMesh; }

	// Returns the cached component mesh
	FORCEINLINE TSubclassOf<class ANWPProjectile> GetDefaultProjectileClass() const { return DefaultProjectileClass; }

	// Returns if the weapon is automatic
	FORCEINLINE bool IsAutomatic() const { return CadenceConfig == ENWPWeaponCadenceConfig::Automatic || CadenceConfig == ENWPWeaponCadenceConfig::Both; }

	// Returns if the weapon is semi automatic
	FORCEINLINE bool IsSemiAutomatic() const { return CadenceConfig == ENWPWeaponCadenceConfig::SemiAutomatic || CadenceConfig == ENWPWeaponCadenceConfig::Both; }

	// Returns the cadence config 
	ENWPWeaponCadenceConfig GetCadenceConfig() const { return CadenceConfig; };

	// Returns the cadence type 
	ENWPWeaponCadenceType GetCadenceType() const;

	// Returns the weapon cool down for a cadence type
	FORCEINLINE float GetCoolDownForCadenceType(ENWPWeaponCadenceType _CadenceType) const { return 1.0f / GetMuzzleVelocityForCadenceType(_CadenceType); }

	// Returns a muzzle velocity for a cadence type
	FORCEINLINE float GetMuzzleVelocityForCadenceType(ENWPWeaponCadenceType _CadenceType) const { return MuzzleVelocity[(int32)_CadenceType];  }

	// Returns the initial ammo
	FORCEINLINE int32 GetInitialAmmo() const { return InitialAmmo; };

	// Returns the maximum ammo
	FORCEINLINE int32 GetMaximumAmmo() const { return MaximumAmmo; };

	// Returns the ammo per magazine
	FORCEINLINE int32 GetAmmoPerMagazine() const { return AmmoPerMagazine; };

	// Returns the magazine reload time
	FORCEINLINE int32 GetMagazineReloadTime() const { return MagazineReloadTime; };

	// Returns the shoot distance
	FORCEINLINE float GetShootDistance() const { return ShootDistance; };

	// Returns if the projectile should be used as ammo
	FORCEINLINE bool ShouldUseProjectileAsAmmo() const { return bUseProjectileAsAmmo; };

	// Returns if the eyes should be used as shoot origin
	FORCEINLINE bool ShouldUseEyesAsShootOrigin() const { return bUseEyesAsShootOrigin; };

	// Returns the eyes bone offset location
	FORCEINLINE FVector GetEyesOffsetLocation() const { return EyesOffsetLocation; };

	// Returns the eyes bone offset rotation
	FORCEINLINE FRotator GetEyesOffsetRotation() const { return EyesOffsetRotation; };

	// Returns the muzzle bone name
	FORCEINLINE FString GetMuzzleBoneName() const { return MuzzleSocketName; };

	// Returns the muzzle bone offset location
	FORCEINLINE FVector GetMuzzleBoneOffsetLocation() const { return MuzzleBoneOffsetLocation; };

	// Returns the muzzle bone offset rotation
	FORCEINLINE FRotator GetMuzzleBoneOffsetRotation() const { return MuzzleBoneOffsetRotation; };

	// Returns the effect spawned at the muzzle each time we shot
	FORCEINLINE UParticleSystem* GetMuzzleEffect() const { return CachedMuzzleEffect; };

	// Returns the sound to play each time we shot
	FORCEINLINE USoundBase* GetShootSound() const { return CachedShootSound; };

	// Returns the montage that has to be played when shooting
	FORCEINLINE UAnimMontage* GetShootingMontage() const { return CachedShootingMontage; };

	////////////////////////////////////////////////////////////////
	// Load / Unload

	// Loads the weapon config synchronously
	// TODO: [NWP-REVIEW] Make this load asynchronously
	void LoadWeaponConfig(bool bSyncLoad = true, const FNWPOnWeaponConfigLoadedDelegate& _Callback = nullptr);

	// Releases the asset hard references
	void ReleaseWeaponConfig();

protected:

	// Finishes the weapon config load
	void FinishWeaponConfigLoad();
	
// Member variables
protected:

	///////////////////////////////////////////////////////////////////////////
	// Configuration

	// Specifies the skeletal mesh for this weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Configuration")
	TSoftObjectPtr<class USkeletalMesh> WeaponMesh;

	// Default projectile to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Configuration")
	TSubclassOf<class ANWPProjectile> DefaultProjectileClass;

	// Specifies the cadence config for this weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Configuration")
	ENWPWeaponCadenceConfig CadenceConfig;

	// Specifies the muzzle velocity for a cadence type
	UPROPERTY(EditAnywhere, Category = "Weapon Configuration")
	float MuzzleVelocity[(int32)ENWPWeaponCadenceType::COUNT];

	// Specifies the ammo when spawned
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Configuration")
	int32 InitialAmmo;

	// Specifies the maximum ammount of ammo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Configuration")
	int32 MaximumAmmo;

	// Specifies the ammount of ammo per magazine
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Configuration")
	int32 AmmoPerMagazine;

	// Specifies the magazine reload time
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Configuration")
	float MagazineReloadTime;

	// Specifies the shoot distance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Configuration")
	float ShootDistance;

	// Specifies that the ammo that is going to be used is the projectile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Configuration")
	bool bUseProjectileAsAmmo;

	// Specifies that the shoot origin is the character eyes. If the shoot origin is not the eyes, the muzzle will be used.
	// Be careful when shooting a projectile from the muzzle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Configuration")
	bool bUseEyesAsShootOrigin;

	// Specifies the muzzle bone name. If the muzzle socket does not exist, the shoot will be done from the eyes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Configuration")
	FString MuzzleSocketName;

	// Specifies an offset location that is going to be added to the eyes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Configuration")
	FVector EyesOffsetLocation;

	// Specifies an offset rotation that is going to be added to the eyes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Configuration")
	FRotator EyesOffsetRotation;

	// Specifies an offset location that is going to be added to the muzzle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Configuration")
	FVector MuzzleBoneOffsetLocation;

	// Specifies an offset rotation that is going to be added to the muzzle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Configuration")
	FRotator MuzzleBoneOffsetRotation;

	// Effect spawned at the muzzle each time we shot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Configuration")
	TSoftObjectPtr<UParticleSystem> MuzzleEffect;

	// Sound to play each time we shot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Configuration")
	TSoftObjectPtr<USoundBase> ShootSound;

	// Specifies the montage that has to be played when shooting
	UPROPERTY(EditAnywhere, Category = "Weapon Configuration")
	TSoftObjectPtr<UAnimMontage> ShootMontage;

	////////////////////////////////////////////////////////////////
	// Load / Unload

	// Delegate for the load finish
	FNWPOnWeaponConfigLoadedDelegate OnCurrentLoadFinishedDelegate;

	// Indicates that the weapon config is being loaded
	bool bIsLoading;

	///////////////////////////////////////////////////////////////////////////
	// Cache

	// Cached skeletal mesh
	UPROPERTY(Transient, SkipSerialization)
	class USkeletalMesh* CachedWeaponMesh;

	// Cached muzzle effect
	UPROPERTY(Transient, SkipSerialization)
	class UParticleSystem* CachedMuzzleEffect;

	// Cached shoot sound
	UPROPERTY(Transient, SkipSerialization)
	class USoundBase* CachedShootSound;

	// Cached shooting montage
	UPROPERTY(Transient, SkipSerialization)
	class UAnimMontage* CachedShootingMontage;

	// Cached reloading montage
	UPROPERTY(Transient, SkipSerialization)
	class UAnimMontage* CachedReloadingMontage;

};
