// Copyright 2019 Neuron Station. All Rights Reserved.

#include "NWPWeapon.h"

// UE
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"

// NWP
#include "NeuronTestCharacter.h"

ANWPWeapon::ANWPWeapon(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	// Create a gun mesh component
	FirstPersonGun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonGun"));
	//FirstPersonGun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FirstPersonGun->bCastDynamicShadow = false;
	FirstPersonGun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FirstPersonGun->SetupAttachment(RootComponent);

	// Initialize members
	CurrentWeaponConfig = nullptr;
	CurrentWeaponState = ENWPWeaponState::Invalid;
	CurrentCoolDown = 0.0f;
	CurrentAmmo = 0;
	CurrentAmmoInMagazine = 0;
	WeaponStateBeforeReload = ENWPWeaponState::Invalid;
}

void ANWPWeapon::BeginPlay()
{
	Super::BeginPlay();

	// Set the state to none
	SetWeaponState(ENWPWeaponState::None);
}

void ANWPWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Execute the update methods 
	UpdateCoolDown(DeltaSeconds);

	UpdateShootingState(DeltaSeconds);
}

void ANWPWeapon::LoadWeapon(TSubclassOf<class UNWPWeaponConfig> _WeaponConfig)
{
	// Check if the weapon is currently configured
	if (CurrentWeaponConfig)
	{
		// Release if configured
		ReleaseWeapon();
	}

	// TODO: Revise this
	CurrentWeaponConfig = NewObject<UNWPWeaponConfig>(GetWorld(), _WeaponConfig.Get() ? _WeaponConfig : DefaultWeaponConfigClass);

	// Load the weapon config
	FNWPOnWaponConfigLoaded WeaponCofigLoadedDelegate = FNWPOnWaponConfigLoaded::CreateUObject(this, &ANWPWeapon::OnWeaponLoaded);
	CurrentWeaponConfig->LoadWeaponConfig(true, WeaponCofigLoadedDelegate);
}

void ANWPWeapon::ReleaseWeapon()
{
	// Check if the weapon is currently configured
	if (CurrentWeaponConfig)
	{
		CurrentWeaponConfig->ReleaseWeaponConfig();
		CurrentWeaponConfig = nullptr;
	}
}

void ANWPWeapon::SwapCadenceType()
{
	// Early return if weapon busy or cooling down
	if (CurrentWeaponState != ENWPWeaponState::None || IsCoolDownActive())
	{
		return;
	}

	// Swap between automatic / semi automatic
	if (CurrentWeaponConfig && CurrentWeaponState != ENWPWeaponState::Reloading)
	{
		if (CurrentConfiguredCadenceType == ENWPWeaponCadenceType::Automatic && CurrentWeaponConfig->IsSemiAutomatic())
		{
			CurrentConfiguredCadenceType = ENWPWeaponCadenceType::SemiAutomatic;
		}
		else if (CurrentConfiguredCadenceType == ENWPWeaponCadenceType::SemiAutomatic && CurrentWeaponConfig->IsAutomatic())
		{
			CurrentConfiguredCadenceType = ENWPWeaponCadenceType::Automatic;
		}
	}
}

void ANWPWeapon::SetOwnerCharacter(class ANeuronTestCharacter* _NewOwnerCharacter, bool _bAttachToOwner)
{
	// Set the new value
	OwnerCharacter = _NewOwnerCharacter;

	// Attach to the owner if required
	if (_bAttachToOwner)
	{
		AttachToOwner();
	}
}

void ANWPWeapon::AttachToOwner()
{
	USkeletalMeshComponent* OwnerSkelletalMesh = OwnerCharacter->GetNWPSkelletalMeshComponent();
	check(OwnerSkelletalMesh);

	// Attach gun actor to owner skelleton
	AttachToComponent(OwnerSkelletalMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
}

void ANWPWeapon::DetachFromOwner()
{
	// Detach the root component from his parent
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void ANWPWeapon::StartShooting()
{
	// Return if weapon busy, cooling down or invalid weapon config
	if (CurrentWeaponState != ENWPWeaponState::None || IsCoolDownActive())
	{
		return;
	}

	// Evaluate if the weapon is automatic / semi automatic
	if (CurrentWeaponConfig)
	{
		// Always execute a shoot step
		if (!InternalShootStep() && CurrentWeaponConfig->IsAutomatic())
		{
			// Set the shooting state
			SetWeaponState(ENWPWeaponState::Shooting);
		}
	}
}

void ANWPWeapon::StopShooting()
{
	// Set to force the none state if reloading
	if (CurrentWeaponState == ENWPWeaponState::Reloading)
	{
		bForceReloadToNone = true;
		return;
	}

	// Check if the state is shooting
	if (CurrentWeaponState != ENWPWeaponState::Shooting)
	{
		return;
	}

	// Set the none state
	SetWeaponState(ENWPWeaponState::None);
}

void ANWPWeapon::OnWeaponLoaded()
{
	// Check if the weapon is configured
	if (CurrentWeaponConfig)
	{
		// Configure the components
		FirstPersonGun->SetSkeletalMesh(CurrentWeaponConfig->GetWeaponMesh());

		// Cache some variables
		CurrentConfiguredCadenceType = CurrentWeaponConfig->GetCadenceType();
		CurrentAmmo = FMath::Min(CurrentWeaponConfig->GetInitialAmmo(), CurrentWeaponConfig->GetMaximumAmmo());
		CurrentAmmoInMagazine = CurrentWeaponConfig->GetAmmoPerMagazine();
	}
}

void ANWPWeapon::SetWeaponState(ENWPWeaponState _WeaponStateToSet)
{
	// Check if the state to set is different
	if (CurrentWeaponState == _WeaponStateToSet)
	{
		return;
	}

	// Change the weapon state & execute the callback
	CurrentWeaponState = _WeaponStateToSet;
	OnWeaponStateChanged();
}

void ANWPWeapon::OnWeaponStateChanged()
{
	// Check the weapon state
	switch (CurrentWeaponState)
	{
	case ENWPWeaponState::Shooting:

		if (OwnerCharacter)
		{
			OwnerCharacter->OnStartShoot();
		}

		break;
	}
}

void ANWPWeapon::UpdateShootingState(float DeltaSeconds)
{
	// Check if the state is shooting
	if (CurrentWeaponState != ENWPWeaponState::Shooting || IsCoolDownActive())
	{
		return;
	}

	// Execute a shoot step
	InternalShootStep();
}

bool ANWPWeapon::InternalShootStep()
{
	// Try to consume the ammo
	if (TryToConsumeAmmo())
	{
		// Spawn the projectile
		SpawProjectile();

		// Reset cool down
		ResetCoolDown();

		return false;
	}
	else
	{
		// Otherwise, try to reload
		CheckMagazineHasToReload();

		return true;
	}
}

bool ANWPWeapon::IsCoolDownActive()
{
	return CurrentCoolDown > 0.0f;
}

void ANWPWeapon::ResetCoolDown()
{
	// Evaluate the wapon config
	if (CurrentWeaponConfig)
	{
		SetCoolDown(CurrentWeaponConfig->GetCoolDownForCadenceType(CurrentConfiguredCadenceType));
	}
}

void ANWPWeapon::SetCoolDown(float Value, bool bAdditive /*= false*/)
{
	CurrentCoolDown = !bAdditive ? Value : (CurrentCoolDown + bAdditive);
}

void ANWPWeapon::UpdateCoolDown(float DeltaTime)
{
	CurrentCoolDown -= DeltaTime;
	CurrentCoolDown = FMath::Max(CurrentCoolDown, 0.0f);

	// Set the state to none if required
	if (CurrentCoolDown == 0)
	{
		// Check if reloading
		if (CurrentWeaponState == ENWPWeaponState::Reloading)
		{
			// Perform the reload
			ReloadMagazine();

			// Check if the state should be none
			if (!bForceReloadToNone)
			{
				// Restore the state before reloading
				SetWeaponState(WeaponStateBeforeReload);
			}
			else
			{
				SetWeaponState(ENWPWeaponState::None);
			}
		}
	}
}

bool ANWPWeapon::TryToConsumeAmmo()
{
	// Returns if there is enough ammo in the magazine
	if (CurrentAmmoInMagazine > 0)
	{
		CurrentAmmoInMagazine = FMath::Max(0, --CurrentAmmoInMagazine);
		return true;
	}

	return false;
}

void ANWPWeapon::CheckMagazineHasToReload()
{
	// Return if no ammo or no weapon config
	if (CurrentAmmo == 0 || !CurrentWeaponConfig)
	{
		return;
	}

	// Set the cool down to the reload time
	if (CurrentWeaponConfig)
	{
		SetCoolDown(CurrentWeaponConfig->GetMagazineReloadTime());
	}

	// Cache current state
	WeaponStateBeforeReload = CurrentWeaponState;

	// Reset the force to reload flag
	bForceReloadToNone = false;

	// Set the reload state
	SetWeaponState(ENWPWeaponState::Reloading);
}

void ANWPWeapon::ReloadMagazine()
{
	// Calculate the ammo to reload
	float AmmoDelta = CurrentWeaponConfig->GetAmmoPerMagazine() - CurrentAmmoInMagazine;
	float AmmoToReload = CurrentAmmo >= AmmoDelta ? AmmoDelta : CurrentAmmo;

	CurrentAmmo -= AmmoToReload;
	CurrentAmmoInMagazine += AmmoToReload;
}
	
void ANWPWeapon::SpawProjectile()
{
	// Spawn projectile if configured
	if (OwnerCharacter && CurrentWeaponConfig)
	{
		UWorld* World = GetWorld();

		if (World)
		{
			// Calculate the spawn position / rotation
			FVector SpawnLocation;
			FRotator SpawnRotation;
			bool bMuzzleSocketIsValid = FirstPersonGun->DoesSocketExist(FName(*CurrentWeaponConfig->GetMuzzleBoneName()));
			FTransform MuzzleTransform;

			// Calculate muzzle transform
			if(bMuzzleSocketIsValid)
			{
				// Add the offsets to the socket transform
				MuzzleTransform = FirstPersonGun->GetSocketTransform(FName(*CurrentWeaponConfig->GetMuzzleBoneName()));
				FVector TransformedOffset = MuzzleTransform.TransformVector(CurrentWeaponConfig->GetMuzzleBoneOffsetLocation());
				MuzzleTransform.SetLocation(MuzzleTransform.GetLocation() + TransformedOffset);
				MuzzleTransform.SetRotation((MuzzleTransform.GetRotation().Rotator() + CurrentWeaponConfig->GetMuzzleBoneOffsetRotation()).Quaternion());
			}

			// Check if the socket exists & the shot is from the muzzle
			if (bMuzzleSocketIsValid && !CurrentWeaponConfig->ShouldUseEyesAsShootOrigin())
			{
				SpawnLocation = MuzzleTransform.GetLocation();
				SpawnRotation = MuzzleTransform.GetRotation().Rotator();
			}
			else
			{
				// Get the eyes location / rotation
				OwnerCharacter->GetActorEyesViewPoint(SpawnLocation, SpawnRotation);
				const FTransform EyesTransform = FTransform(SpawnRotation, SpawnLocation);
				SpawnLocation += EyesTransform.TransformVector(CurrentWeaponConfig->GetEyesOffsetLocation());
				SpawnRotation += CurrentWeaponConfig->GetEyesOffsetRotation();
			}

			// Calculate end position
			FVector EndPosition = SpawnLocation + SpawnRotation.Vector() * CurrentWeaponConfig->GetShootDistance();

#if !UE_BUILD_SHIPPING 
			// Draw the expected trajectory
			if (CVarbDebugWeapon.GetValueOnGameThread())
			{
				DrawDebugLine(World, SpawnLocation, EndPosition, FColor::Green, false, 1.0f, 0.0f, 3.0f);
			}	
#endif

			// Check if a projectile has to be spawned
			if (CurrentWeaponConfig->ShouldUseProjectileAsAmmo())
			{
				// Spawn the projectile
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// TODO: [NWP-REVIEW] Implement a projectile pool
				ANWPProjectile* SpawnedProjectile = World->SpawnActor<ANWPProjectile>(CurrentWeaponConfig->GetDefaultProjectileClass(), SpawnLocation, SpawnRotation, ActorSpawnParams);

				// Set the owner weapon
				SpawnedProjectile->SetOwnerWeapon(this);

				// Add the projectile to the projectiles list
				CurrentSpawnedProjectiles.Add(SpawnedProjectile);
			}
			else
			{
				// Shoot a ray
				FCollisionQueryParams QueryParams;

				// Ignore the weapon & character
				QueryParams.AddIgnoredActor(this);
				QueryParams.AddIgnoredActor(OwnerCharacter);

				QueryParams.bTraceComplex = true;

				FHitResult Hit;

				// Shoot a ray from the projectile to the target
				if (World->LineTraceSingleByChannel(Hit, SpawnLocation, EndPosition, COLLISION_WEAPON, QueryParams))
				{
					UE_LOG(LogNWP, Log, TEXT("Shot has hit: HitActor: %s HitComponent: %s "), *Hit.GetActor()->GetName(), *Hit.GetComponent()->GetName());
				}
			}

			// Spawn the shot effect & muzzle sound at the muzzle if possible
			if (bMuzzleSocketIsValid)
			{
				if (CurrentWeaponConfig->GetMuzzleEffect() != nullptr)
				{
					UGameplayStatics::SpawnEmitterAttached(CurrentWeaponConfig->GetMuzzleEffect(), FirstPersonGun, FName(*CurrentWeaponConfig->GetMuzzleBoneName()));
				}

				if (CurrentWeaponConfig->GetShootSound() != nullptr)
				{
					UGameplayStatics::PlaySoundAtLocation(World, CurrentWeaponConfig->GetShootSound(), MuzzleTransform.GetLocation());
				}
			}
		}
	}
}

void ANWPWeapon::OnProjectileIsGoingToBeDestroyed(ANWPProjectile* _ProjectileToProcess)
{
	// Remove the projectile from the list
	CurrentSpawnedProjectiles.Remove(_ProjectileToProcess);
}
