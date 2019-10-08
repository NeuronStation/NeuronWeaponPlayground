// // Plugin that implements a simple and scalable weapon system developed by Neuron Station

#pragma once

// NWP
#include "NeuronTestCharacter.h"
#include "NWPWeaponConfig.h"
#include "NWPProjectile.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NWPWeapon.generated.h"

/**
 * Basic class for a weapon. It can shoot & reload. It has support for ammo (including projectiles). Can be configured using UNWPWeaponConfig
 */
UCLASS()
class NEURONWEAPONPLAYGROUND_API ANWPWeapon : public AActor
{
	GENERATED_BODY()

// Friend class
friend class ANWPProjectile;

// Constructors
public:

	ANWPWeapon(const class FObjectInitializer& ObjectInitializer);

// Member functions
public:

	/// AActor interface begin
	// Overridable native event for when play begins for this actor.
	virtual void BeginPlay() override;
	
	// Function called every frame on this Actor. 
	virtual void Tick(float DeltaSeconds) override;
	/// AActor interface end

	///////////////////////////////////////////////////////////////////////////
	// Accessors

	// Returns the weapon config
	FORCEINLINE const class UNWPWeaponConfig* GetWeaponConfig() const { return CurrentWeaponConfig; }

	// Returns the weapon current state 
	FORCEINLINE ENWPWeaponState GetWeaponState() const { return CurrentWeaponState; }

	// Returns the weapon current cadence type
	FORCEINLINE ENWPWeaponCadenceType GetCurrentConfiguredCadenceType() const { return CurrentConfiguredCadenceType; }

	// Returns the weapon current cooldown
	FORCEINLINE float GetCurrentCoolDown() const { return CurrentCoolDown; }

	// Returns the weapon current ammo
	FORCEINLINE int32 GetCurrentAmmo() const { return CurrentAmmo; }

	// Returns the weapon current ammo in magazine
	FORCEINLINE int32 GetCurrentAmmoInMagazine() const { return CurrentAmmoInMagazine; }

	///////////////////////////////////////////////////////////////////////////
	// Load / Unload

	// Loads the required weapon assets
	void LoadWeapon(TSubclassOf<class UNWPWeaponConfig> _WeaponConfig);

	// Releases the loaded weapon assets
	void ReleaseWeapon();

	///////////////////////////////////////////////////////////////////////////
	// Weapon State

	// Returns if the weapon is shooting
	FORCEINLINE bool IsShooting() const { return CurrentWeaponState == ENWPWeaponState::Shooting; }

	// Returns if the weapon is reloading
	FORCEINLINE bool IsReloading() const { return CurrentWeaponState == ENWPWeaponState::Reloading; }

	// Changes the cadence type
	void SwapCadenceType();

	///////////////////////////////////////////////////////////////////////////
	// Attach

	// Returns if the weapon has an owner
	FORCEINLINE bool HasOwner() const { return OwnerCharacter != nullptr; }

	// Returns the weapon owner
	FORCEINLINE class ANeuronTestCharacter* GetCharacterOwner() const { return OwnerCharacter; }

	// This function that sets the owner character for this weapon
	void SetOwnerCharacter(class ANeuronTestCharacter* _NewOwnerCharacter, bool _bAttachToOwner = true);

	// Attaches the weapon to the owner
	void AttachToOwner();

	// Detaches the weapon from the owner
	void DetachFromOwner();

	///////////////////////////////////////////////////////////////////////////
	// Shoot

	// Starts the shooting process
	void StartShooting();

	// Stops the shooting process
	void StopShooting();

protected:

	///////////////////////////////////////////////////////////////////////////
	// Load / Unload

	// Callback executed when the weapon loads
	virtual void OnWeaponLoaded();

	///////////////////////////////////////////////////////////////////////////
	// Weapon State

	// Set the weapon state
	void SetWeaponState(ENWPWeaponState _WeaponStateToSet);

	// Callback called when the weapon state has just changed
	void OnWeaponStateChanged();

	// Updates the shooting state
	void UpdateShootingState(float DeltaSeconds);

	///////////////////////////////////////////////////////////////////////////
	// Shoot

	// Executes a shoot step. Returns if the shoot step has caused a reload
	bool InternalShootStep();

	///////////////////////////////////////////////////////////////////////////
	// Cool down

	// Returns if the weapon cool down is active
	bool IsCoolDownActive();

	// Reset the cool down using the cadence type
	void ResetCoolDown();

	// Sets a new value for the cool down
	void SetCoolDown(float Value, bool bAdditive = false);

	// Updates the weapon cool down
	void UpdateCoolDown(float DeltaTime);

	///////////////////////////////////////////////////////////////////////////
	// Ammo

	// Tries to consume the ammo
	bool TryToConsumeAmmo();

	// Checks if the magazine has to reload
	void CheckMagazineHasToReload();

	// Reloads the magazine using the current ammo
	void ReloadMagazine();

	///////////////////////////////////////////////////////////////////////////
	// Projectile

	// Spawns the projectile
	virtual void SpawProjectile();

	// Callback executed after the projectile velocity has been computed
	virtual void OnProjectileVelocityComputed(class ANWPProjectile* _ProjectileToProcess, FVector& _ComputedVelocity, float DeltaTime) {};

	// Callback executed when the projectile has hit something
	virtual void OnProjectileHit(class ANWPProjectile* _ProjectileToProcess, class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit) {};

	// Callback executed before the projectile is destroyed
	virtual void OnProjectileIsGoingToBeDestroyed(class ANWPProjectile* _ProjectileToProcess);

// Member variables
protected:

	////////////////////////////////////////////////////////////////
	// Components

	// First person view (seen only by self) 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* FirstPersonGun;

	///////////////////////////////////////////////////////////////////////////
	// Configuration

	// Default weapon configuration class to create
	UPROPERTY(EditDefaultsOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UNWPWeaponConfig> DefaultWeaponConfigClass;

	///////////////////////////////////////////////////////////////////////////
	// State

	// Reference to the owning character
	UPROPERTY(Transient, SkipSerialization)
	class ANeuronTestCharacter* OwnerCharacter;

	// Current weapon config
	UPROPERTY(Transient, SkipSerialization)
	class UNWPWeaponConfig* CurrentWeaponConfig;

	// Current weapon state
	UPROPERTY(Transient, SkipSerialization)
	ENWPWeaponState CurrentWeaponState;

	// Current cadence configured
	UPROPERTY(Transient, SkipSerialization)
	ENWPWeaponCadenceType CurrentConfiguredCadenceType;

	// The current value of the cooldown
	UPROPERTY(Transient, SkipSerialization)
	float CurrentCoolDown;

	// The current amount of ammo 
	UPROPERTY(Transient, SkipSerialization)
	int32 CurrentAmmo;

	// The current amount of ammo in the magazine
	UPROPERTY(BlueprintReadOnly, Transient, SkipSerialization)
	int32 CurrentAmmoInMagazine;

	// The weapon state before the reload
	UPROPERTY(Transient, SkipSerialization)
	ENWPWeaponState WeaponStateBeforeReload;

	// When finishing a reload, forces the none state
	UPROPERTY(Transient, SkipSerialization)
	bool bForceReloadToNone;

	// Current spawned projectiles 
	UPROPERTY(Transient, SkipSerialization)
	TArray<ANWPProjectile*> CurrentSpawnedProjectiles;
};
