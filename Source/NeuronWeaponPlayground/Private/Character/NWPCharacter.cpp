// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

//Class
#include "NWPCharacter.h"

// NWP
#include "NWPAnimInstanceCharacter.h"
#include "NWPWeapon.h"
#include "NWPWeaponConfig.h"

// UE
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

ANWPCharacter::ANWPCharacter(const class FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	BaseTurnRate(45.0f),
	BaseLookUpRate(45.0f),
	FirstPersonMeshComponent(nullptr),
	FirstPersonCameraComponent(nullptr),
	CurrentWeapon(nullptr),
	CurrentWeaponIndex(-1)

{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCameraComponent"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.0f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a first person view (when controlling this pawn)
	FirstPersonMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMeshComponent"));
	FirstPersonMeshComponent->SetOnlyOwnerSee(true);
	FirstPersonMeshComponent->SetupAttachment(FirstPersonCameraComponent);
	FirstPersonMeshComponent->bCastDynamicShadow = false;
	FirstPersonMeshComponent->CastShadow = false;
	FirstPersonMeshComponent->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	FirstPersonMeshComponent->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

FVector ANWPCharacter::GetPawnViewLocation() const
{
	// Return the location of the camera if it exists
	if (FirstPersonCameraComponent)
	{
		return FirstPersonCameraComponent->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Select the first weapon
	SelectWeaponByIndex(0);

	// Configure the first person mesh component
	FirstPersonMeshComponent->SetHiddenInGame(false, true);
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire events
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ANWPCharacter::OnFireStart);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ANWPCharacter::OnFireStop);

	// Bind swap cadence type
	PlayerInputComponent->BindAction("SwapCadence", IE_Pressed, this, &ANWPCharacter::OnSwapCadenceType);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ANWPCharacter::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ANWPCharacter::OnMoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "Turn" handles devices that provide an absolute delta, such as a mouse.
	// "TurnRate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ANWPCharacter::OnTurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ANWPCharacter::OnLookUpAtRate);

	// Weapon inventory
	PlayerInputComponent->BindAction("SelectNextWeapon", IE_Pressed, this, &ANWPCharacter::OnSelectNextWeapon);
	PlayerInputComponent->BindAction("SelectPrevioustWeapon", IE_Pressed, this, &ANWPCharacter::OnSelectPreviousWeapon);
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

UNWPAnimInstanceCharacter* ANWPCharacter::GetNWPAnimInstance() const
{
	return FirstPersonMeshComponent ? Cast<UNWPAnimInstanceCharacter>(FirstPersonMeshComponent->GetAnimInstance()) : nullptr;
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPCharacter::OnFireStart()
{
	// Start shooting the weapon
	if (CurrentWeapon)
	{
		CurrentWeapon->StartShooting();
	}
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPCharacter::OnFireStop()
{
	// Stop shooting the weapon
	if (CurrentWeapon)
	{
		CurrentWeapon->StopShooting();
	}
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPCharacter::OnSwapCadenceType()
{
	// Swap the cadence type
	if (CurrentWeapon)
	{
		CurrentWeapon->SwapCadenceType();
	}
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPCharacter::OnMoveForward(float _ForwardMove)
{
	if (_ForwardMove != 0.0f)
	{
		// Add movement in the forward/backward direction
		AddMovementInput(GetActorForwardVector(), _ForwardMove);
	}
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPCharacter::OnMoveRight(float _RightMove)
{
	if (_RightMove != 0.0f)
	{
		// Add movement in the left/right direction
		AddMovementInput(GetActorRightVector(), _RightMove);
	}
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPCharacter::OnTurnAtRate(float _TurnUpRate)
{
	// Calculate delta for this frame from the rate information
	AddControllerYawInput(_TurnUpRate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPCharacter::OnLookUpAtRate(float _LookUpRate)
{
	// Calculate delta for this frame from the rate information
	AddControllerPitchInput(_LookUpRate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPCharacter::OnSelectNextWeapon()
{
	SelectWeaponByIndex(++CurrentWeaponIndex % DefaultWeaponClasses.Num());
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPCharacter::OnSelectPreviousWeapon()
{
	SelectWeaponByIndex((--CurrentWeaponIndex >= 0 ? CurrentWeaponIndex : DefaultWeaponClasses.Num() - 1) % DefaultWeaponClasses.Num());
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPCharacter::SelectWeaponByIndex(int32 _NewWeaponIndex)
{
	// TODO: [NWP-REVIEW] The weapons should be spawned when the character is created. Then, the weapons will be disabled/enabled 
	// according to which one is being used. This is a faster implementation. As a new weapon is spawned, the ammo is regenerated and the cooldown is set to 0 

	// Early return if invalid index
	if (_NewWeaponIndex < 0 || _NewWeaponIndex > DefaultWeaponClasses.Num())
	{
		return;
	}

	// Destroy current weapon
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
	}

	// Spawn the selected weapon
	UWorld* World = GetWorld();

	if (DefaultWeaponClasses[_NewWeaponIndex] && World)
	{
		// Spawn the weapon
		CurrentWeapon = World->SpawnActor<ANWPWeapon>(DefaultWeaponClasses[_NewWeaponIndex], FVector::ZeroVector, FRotator::ZeroRotator);
		CurrentWeaponIndex = _NewWeaponIndex;

		// Set the weapon owner
		CurrentWeapon->SetOwnerCharacter(this);

		// Load the weapon using a dummy config, that will load the default configuration
		TSubclassOf<class UNWPWeaponConfig> WeaponConfig;
		CurrentWeapon->LoadWeapon(WeaponConfig);
	}
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPCharacter::OnStartShoot()
{
	UNWPAnimInstanceCharacter* NWPAnimInstance = GetNWPAnimInstance();

	// Reproduce start shooting animation
	if (NWPAnimInstance && CurrentWeapon && CurrentWeapon->GetWeaponConfig())
	{
		NWPAnimInstance->PlayActionMontage(CurrentWeapon->GetWeaponConfig()->GetShootingMontage());
	}
}
