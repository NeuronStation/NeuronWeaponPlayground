// Copyright 2019 Neuron Station. All Rights Reserved.

#pragma once

// NWP
#include "NeuronWeaponPlayground.h"

// UE
#include "AITypes.h"
#include "Navigation/PathFollowingComponent.h"

// Generated
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NWPAIPathComponent.generated.h"

/**
 * AI path component that contains a list of waypoints relevant for the ai.
 * The pawn can only move to the waypoints only if the waypoint behavior has started.
 */
UCLASS( ClassGroup=(AI), meta=(BlueprintSpawnableComponent) )
class NEURONWEAPONPLAYGROUND_API UNWPAIPathComponent : public UActorComponent
{
	GENERATED_BODY()

// Constructors
public:

	UNWPAIPathComponent(const class FObjectInitializer& ObjectInitializer);

// Member functions
public:

	// Returns if the waypoint behavior is active
	UFUNCTION(BlueprintCallable, Category = "AI")
	FORCEINLINE	bool IsWaypointBehaviorActive() const { return bIsWaypointBehaviorActive; }

	// Returns the current waypoint of the path
	UFUNCTION(BlueprintCallable, Category = "AI")
	class ATargetPoint* GetCurrentWaypoint() const;

	// Returns the previous waypoint of the path
	UFUNCTION(BlueprintCallable, Category = "AI")
	class ATargetPoint* GetPreviousWaypoint() const;

	// Returns the next waypoint of the path
	UFUNCTION(BlueprintCallable, Category = "AI")
	class ATargetPoint* GetNextWaypoint() const;

	// Returns the current waypoint index of the path
	UFUNCTION(BlueprintCallable, Category = "AI")
	int32 GetCurrentWaypointIndex() const;

	// Returns the previous waypoint index of the path
	UFUNCTION(BlueprintCallable, Category = "AI")
	int32 GetPreviousWaypointIndex() const;

	// Returns the next waypoint index of the path
	UFUNCTION(BlueprintCallable, Category = "AI")
	int32 GetNextWaypointIndex() const;

	// Starts the waypoint follow behavior on the pawn
	UFUNCTION(BlueprintCallable, Category = "AI")
	void StartWaypointBehavior(ENWPWaypointBehaviorStartType WaypointBehaviorStartType = ENWPWaypointBehaviorStartType::UseFirst,
			int32 WeaponBehaviorIndex = -1);

	// Stops the waypoint follow behavior on the pawn
	UFUNCTION(BlueprintCallable, Category = "AI")
	void StopWaypointBehavior();

	// Moves the pawn to the previous waypoint
	UFUNCTION(BlueprintCallable, Category = "AI")
	void MoveToPreviousWaypoint();

	// Moves the pawn to the next waypoint
	UFUNCTION(BlueprintCallable, Category = "AI")
	void MoveToNextWaypoint();

protected:

	/// UActorComponent interface begin
	// Begins Play for the component
	virtual void BeginPlay() override;

	// Ends gameplay for this component
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	/// UActorComponent interface end

	// Moves to a waypoint by index
	void MoveToWaypointByIndex(int32 WaypointIndex);

	// Callback executed when the actor reaches a waypoint
	UFUNCTION()
	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type MovementResult);

// Member variables
protected:

	// List of waypoints which make a path
	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TArray<class ATargetPoint*> WaypointsPath;

	// Indicates if the path is closed. That means that the previous waypoint from the first one is the last one and vice versa
	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	bool bIsClosedPath;

	// Radius used to determine if the pawn has reached the moving to waypoint
	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float AcceptanceRadius;

	// The owner pawn of the component
	UPROPERTY(Transient, SkipSerialization)
	class APawn* OwnerPawn;

	// Indicates if the waypoint behavior is active
	UPROPERTY(Transient, SkipSerialization)
	bool bIsWaypointBehaviorActive;

	// The current waypoint index
	UPROPERTY(Transient, SkipSerialization)
	int32 CurrentWaypointIndex;

	// The moving to waypoint index
	UPROPERTY(Transient, SkipSerialization)
	int32 MovingToWaypointIndex;

	// The cached waypoint index when the waypoint behavior stops
	UPROPERTY(Transient, SkipSerialization)
	int32 StopWaypointIndex;
};
