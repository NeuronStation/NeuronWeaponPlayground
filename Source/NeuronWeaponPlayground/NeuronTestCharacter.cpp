// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "NeuronTestCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId

// NWP
#include "NWPAnimInstanceCharacter.h"
#include "NWPWeapon.h"
#include "NWPWeaponConfig.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ANeuronTestCharacter

ANeuronTestCharacter::ANeuronTestCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	//FP_MuzzleLocation->SetupAttachment(FP_Gun);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;

	// Initialize some variables
	CurrentWeaponIndex = -1;
}

void ANeuronTestCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Select the first weapon
	SelectWeaponByIndex(0);

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ANeuronTestCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire events
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ANeuronTestCharacter::OnFireStart);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ANeuronTestCharacter::OnFireStop);

	// Bind swap cadence type
	PlayerInputComponent->BindAction("SwapCadence", IE_Pressed, this, &ANeuronTestCharacter::SwapCadenceType);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ANeuronTestCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ANeuronTestCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ANeuronTestCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ANeuronTestCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ANeuronTestCharacter::LookUpAtRate);

	// Weapon inventory
	PlayerInputComponent->BindAction("SelectNextWeapon", IE_Pressed, this, &ANeuronTestCharacter::SelectNextWeapon);
	PlayerInputComponent->BindAction("SelectPrevioustWeapon", IE_Pressed, this, &ANeuronTestCharacter::SelectPreviousWeapon);
}

void ANeuronTestCharacter::OnFireStart()
{
	// Start shooting the weapon
	if (CurrentWeapon)
	{
		CurrentWeapon->StartShooting();
	}
}

void ANeuronTestCharacter::OnFireStop()
{
	// Stop shooting the weapon
	if (CurrentWeapon)
	{
		CurrentWeapon->StopShooting();
	}
}

void ANeuronTestCharacter::SwapCadenceType()
{
	// Swap the cadence type
	if (CurrentWeapon)
	{
		CurrentWeapon->SwapCadenceType();
	}
}

void ANeuronTestCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ANeuronTestCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFireStart();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void ANeuronTestCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void ANeuronTestCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void ANeuronTestCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ANeuronTestCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ANeuronTestCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ANeuronTestCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool ANeuronTestCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ANeuronTestCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &ANeuronTestCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ANeuronTestCharacter::TouchUpdate);
		return true;
	}
	
	return false;
}

FVector ANeuronTestCharacter::GetPawnViewLocation() const
{
	// Return the location of the camera if it exists
	if (FirstPersonCameraComponent)
	{
		return FirstPersonCameraComponent->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

void ANeuronTestCharacter::OnStartShoot()
{
	UNWPAnimInstanceCharacter* NWPAnimInstance = GetNWPAnimInstance();

	// Reproduce start shooting animation
	if (NWPAnimInstance && CurrentWeapon && CurrentWeapon->GetWeaponConfig())
	{
		NWPAnimInstance->PlayActionMontage(CurrentWeapon->GetWeaponConfig()->GetShootingMontage());
	}
}

void ANeuronTestCharacter::SelectNextWeapon()
{
	SelectWeaponByIndex(++CurrentWeaponIndex % DefaultWeaponClasses.Num());
}

void ANeuronTestCharacter::SelectPreviousWeapon()
{
	SelectWeaponByIndex((--CurrentWeaponIndex >= 0 ? CurrentWeaponIndex : DefaultWeaponClasses.Num() - 1) % DefaultWeaponClasses.Num());
}

UNWPAnimInstanceCharacter* ANeuronTestCharacter::GetNWPAnimInstance() const
{
	return Mesh1P ? Cast<UNWPAnimInstanceCharacter>(Mesh1P->GetAnimInstance()) : nullptr;
}

void ANeuronTestCharacter::SelectWeaponByIndex(int32 _NewWeaponIndex)
{
	// TODO: [NWP-REVIEW] The weapons should be spawned when the character is created. Then, the weapons will be disabled / enabled 
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
