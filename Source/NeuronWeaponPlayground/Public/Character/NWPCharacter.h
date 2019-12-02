// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

// NWP
#include "NeuronWeaponPlayground.h"

// UE
#include "GenericTeamAgentInterface.h"

// Generated
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NWPCharacter.generated.h"

// Log category
DECLARE_LOG_CATEGORY_EXTERN(LogANWPCharacter, Log, All);

/**
 * Character of the game. Can have equipped with one weapon and have several weapons in the inventory.
 */
UCLASS(config=Game)
class ANWPCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

// Friend class
friend class ANWPWeapon;

// Constructors
public:
	ANWPCharacter(const class FObjectInitializer& ObjectInitializer);

// Member functions
public:

	/// APAwn interface begin
	// Returns Pawn's eye location 
	virtual FVector GetPawnViewLocation() const;
	/// APAwn interface end

	////////////////////////////////////////////////////////////////
	// Accesors

	// External accessor for the the first person mesh component
	FORCEINLINE class USkeletalMeshComponent* GetFirstPersonMeshComponent() const { return FirstPersonMeshComponent; }

	// External accessor for the the first person camera component
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	// External accessor for the weapon
	FORCEINLINE class ANWPWeapon* GetWeapon() const { return CurrentWeapon; }

protected:

	/// AActor interface begin
	// Overridable native event for when play begins for this actor
	virtual void BeginPlay() override;
	/// AActor interface end

	/// APawn interface begin
	// 	Allows a Pawn to set up custom input bindings. Called upon possession by a PlayerController, using the InputComponent created by CreatePlayerInputComponent()
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	/// APawn interface end

	/// IGenericTeamAgentInterface interface begin
	// Assigns Team Agent to given TeamID
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;

	// Retrieve team identifier in form of FGenericTeamId
	virtual FGenericTeamId GetGenericTeamId() const override;

	// Retrieved owner attitude toward given Other object
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	/// IGenericTeamAgentInterface interface end

	// Internal accessor for the anim instance
	class UNWPAnimInstanceCharacter* GetNWPAnimInstance() const;

	////////////////////////////////////////////////////////////////
	// Input Callbacks

	// Callback executed to start the fire
	void OnFireStart();

	// Callback executed to stop the fire
	void OnFireStop();

	// Callback executed to swap the weapon cadence type
	void OnSwapCadenceType();

	// Callback executed to move forward/backward 
	void OnMoveForward(float _ForwardMove);

	// Callback executed to move left/right 
	void OnMoveRight(float _RightMove);

	// Callback executed to turn at a given rate. The rate is normalized (example: 1.0f means 100% of desired turn rate)
	void OnTurnAtRate(float _TurnUpRate);

	// Callback executed to turn look up/down at a given rate. The rate is normalized (example: 1.0f means 100% of desired turn rate)
	void OnLookUpAtRate(float _LookUpRate);

	// Callback executed to select the next weapon in the inventory
	void OnSelectNextWeapon();

	// Callback executed to select previous weapon in the inventory
	void OnSelectPreviousWeapon();

	////////////////////////////////////////////////////////////////
	// Weapon Inventory

	// Selects a weapon by its index
	void SelectWeaponByIndex(int32 _NewWeaponIndex);

	// Callback executed by the weapon which indicates that the weapon has started shooting
	void OnStartShoot();

// Member variables
protected:

	// Base turn rate (degrees/second). Other scaling may affect final turn rate
	UPROPERTY(EditDefaultsOnly, Category = "Camera", meta = (UIMin = 0, ClampMin = 0, AllowPrivateAccess = "true"))
	float BaseTurnRate;

	// Base look up/down rate (degrees/second). Other scaling may affect final rate
	UPROPERTY(EditDefaultsOnly, Category = "Camera", meta = (UIMin = 0, ClampMin = 0, AllowPrivateAccess = "true"))
	float BaseLookUpRate;

	// First person view mesh. It contains the arms seen only by self
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* FirstPersonMeshComponent;

	// First person view camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	// The character team
	UPROPERTY(EditInstanceOnly, Category = "Team", meta = (AllowPrivateAccess = "true"))
	ENWPCharacterTeam CharacterTeam;

	// Default weapon classes to spawn
	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<class ANWPWeapon>> DefaultWeaponClasses;

	// The current character team
	UPROPERTY(Transient, SkipSerialization)
	FGenericTeamId CurrentCharacterTeam;

	// The current character weapon
	UPROPERTY(Transient, SkipSerialization)
	class ANWPWeapon* CurrentWeapon;

	// The current character weapon index
	UPROPERTY(Transient, SkipSerialization)
	int32 CurrentWeaponIndex;

};

