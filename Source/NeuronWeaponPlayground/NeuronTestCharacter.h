// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NeuronTestCharacter.generated.h"

class UInputComponent;

/**
 * Character of the game. Can have equipped with one weapon and have several weapons in the inventory
 *
 * TODO: [NWP-REVIEW] During development, everything related to RV has been neglected
 */
UCLASS(config=Game)
class ANeuronTestCharacter : public ACharacter
{
	GENERATED_BODY()

// Friend class
friend class ANWPWeapon;

// Member functions
public:

	/// APAwn interface begin
	// Returns Pawn's eye location 
	virtual FVector GetPawnViewLocation() const;
	/// APAwn interface end

	////////////////////////////////////////////////////////////////
	// Accesors

	// External accessor for the skelletal mesh component
	FORCEINLINE class USkeletalMeshComponent* GetNWPSkelletalMeshComponent() const { return Mesh1P; }

	// External accessor for the weapon
	FORCEINLINE class ANWPWeapon* GetWeapon() const { return CurrentWeapon; }

	////////////////////////////////////////////////////////////////
	// Weapon Inventory

	// Selects the next weapon in the inventory
	void SelectNextWeapon();

	// Selects the previous weapon in the inventory
	void SelectPreviousWeapon();

protected:

	// Internal accessor for the anim instance
	class UNWPAnimInstanceCharacter* GetNWPAnimInstance() const;

	////////////////////////////////////////////////////////////////
	// Weapon Inventory

	// Callback executed by the weapon wich indicates that the weapon has started shooting
	void OnStartShoot();

	////////////////////////////////////////////////////////////////
	// Weapon Inventory

	// Selects a weapon by its index
	void SelectWeaponByIndex(int32 _NewWeaponIndex);

// Member variables
protected:

	// Default weapon classes to spawn
	UPROPERTY(EditDefaultsOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<class ANWPWeapon>> DefaultWeaponClasses;

	// The current character weapon
	UPROPERTY(Transient, SkipSerialization)
	ANWPWeapon* CurrentWeapon;

	// The current character weapon index
	UPROPERTY(Transient, SkipSerialization)
	int32 CurrentWeaponIndex;

// ------------------------------------- Unreal C++ FPS API -------------------------------------

public:

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: VR view (attached to the VR controller directly, no arm, just the actual gun) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* VR_Gun;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* L_MotionController;

public:
	ANeuronTestCharacter();

protected:
	virtual void BeginPlay();

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	class USoundBase* FireSound;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint32 bUsingMotionControllers : 1;

protected:
	
	/** Starts the projectile firing. */
	void OnFireStart();

	/** Stops the projectile firing. */
	void OnFireStop();

	/** Swaps the weapon cadence type. */
	void SwapCadenceType();

	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

