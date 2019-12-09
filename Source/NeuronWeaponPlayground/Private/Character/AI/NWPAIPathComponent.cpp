// Copyright 2019 Neuron Station. All Rights Reserved.

// Class
#include "NWPAIPathComponent.h"

// NWP
#include "NWPAIController.h"

// UE
#include "Tasks/AITask_MoveTo.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/TargetPoint.h"

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

UNWPAIPathComponent::UNWPAIPathComponent(const class FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	bIsClosedPath(false),
	AcceptanceRadius(10.0f),
	OwnerPawn(nullptr),
	bIsWaypointBehaviorActive(false),
	CurrentWaypointIndex(-1),
	MovingToWaypointIndex(-1),
	StopWaypointIndex(-1)
{

}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

class ATargetPoint* UNWPAIPathComponent::GetCurrentWaypoint() const
{
	int32 WaypointIndex = GetCurrentWaypointIndex();
	return WaypointIndex >= 0 ? WaypointsPath[WaypointIndex] : nullptr;
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

class ATargetPoint* UNWPAIPathComponent::GetPreviousWaypoint() const
{
	int32 WaypointIndex = GetPreviousWaypointIndex();
	return WaypointIndex >= 0 ? WaypointsPath[WaypointIndex] : nullptr;
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

class ATargetPoint* UNWPAIPathComponent::GetNextWaypoint() const
{
	int32 WaypointIndex = GetNextWaypointIndex();
	return WaypointIndex >= 0 ? WaypointsPath[WaypointIndex] : nullptr;
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

int32 UNWPAIPathComponent::GetCurrentWaypointIndex() const
{
	if (CurrentWaypointIndex < 0)
	{
		return -1;
	}

	return CurrentWaypointIndex;
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

int32 UNWPAIPathComponent::GetPreviousWaypointIndex() const
{
	// Invalid index
	if (CurrentWaypointIndex < 0)
	{
		return -1;
	}

	// Closed path extreme case
	if (bIsClosedPath && CurrentWaypointIndex == 0)
	{
		return WaypointsPath.Num() - 1;
	}
	// Default behavior
	else
	{
		return FMath::Max(CurrentWaypointIndex - 1, 0);
	}
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

int32 UNWPAIPathComponent::GetNextWaypointIndex() const
{
	// Invalid index
	if (CurrentWaypointIndex < 0)
	{
		return -1;
	}

	// Closed path extreme case
	if (bIsClosedPath && CurrentWaypointIndex == WaypointsPath.Num() - 1)
	{
		return 0;
	}
	// Default behavior
	else
	{
		return FMath::Min(CurrentWaypointIndex + 1, WaypointsPath.Num() - 1);
	}
}


///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void UNWPAIPathComponent::StartWaypointBehavior(ENWPWaypointBehaviorStartType WaypointBehaviorStartType /*= ENWPWaypointBehaviorStartType::UseFirst*/, 
	int32 WeaponBehaviorIndex /*= -1*/)
{
	// Return if the waypoint behavior is active
	if (bIsWaypointBehaviorActive)
	{
		return;
	}

	bIsWaypointBehaviorActive = true;

	// Determine the waypoint index to move to according to the start type
	if (WaypointBehaviorStartType == ENWPWaypointBehaviorStartType::ByIndex)
	{
		MoveToWaypointByIndex(WeaponBehaviorIndex);
	}
	else if (WaypointBehaviorStartType == ENWPWaypointBehaviorStartType::UseCache)
	{
		MoveToWaypointByIndex(StopWaypointIndex > 0 ? StopWaypointIndex : 0);
		StopWaypointIndex = -1;
	}
	else
	{
		MoveToWaypointByIndex(0);
	}
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void UNWPAIPathComponent::StopWaypointBehavior()
{
	// Return if the waypoint behavior is not active
	if (!bIsWaypointBehaviorActive)
	{
		return;
	}

	// Stop the pawn movement
	ANWPAIController* AIController = CastChecked<ANWPAIController>(OwnerPawn->GetController());
	AIController->StopMovement();

	// Cache the stop waypoint index
	StopWaypointIndex = MovingToWaypointIndex > 0 ? MovingToWaypointIndex : CurrentWaypointIndex;

	// Reset some variables
	CurrentWaypointIndex = -1;
	MovingToWaypointIndex = -1;
	bIsWaypointBehaviorActive = false;
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void UNWPAIPathComponent::MoveToPreviousWaypoint()
{
	int32 PreviousWaypointIndex = GetPreviousWaypointIndex();
	MoveToWaypointByIndex(PreviousWaypointIndex);
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void UNWPAIPathComponent::MoveToNextWaypoint()
{
	int32 NextWaypointIndex = GetNextWaypointIndex();
	MoveToWaypointByIndex(NextWaypointIndex);
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void UNWPAIPathComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPawn = CastChecked<APawn>(GetOwner());

	// Register to the move completed callback
	ANWPAIController* AIController = Cast<ANWPAIController>(OwnerPawn->GetController());

	if (AIController)
	{
		AIController->ReceiveMoveCompleted.AddDynamic(this, &UNWPAIPathComponent::OnMoveCompleted);
	}
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void UNWPAIPathComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Unregister from the move completed callback
	ANWPAIController* AIController = Cast<ANWPAIController>(OwnerPawn->GetController());

	if (AIController)
	{
		AIController->ReceiveMoveCompleted.RemoveDynamic(this, &UNWPAIPathComponent::OnMoveCompleted);
	}

	OwnerPawn = nullptr;

	Super::EndPlay(EndPlayReason);
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void UNWPAIPathComponent::MoveToWaypointByIndex(int32 WaypointIndex)
{
	// Evaluate the move invariant and valid states
	if (!bIsWaypointBehaviorActive || WaypointIndex < 0 || WaypointIndex >= WaypointsPath.Num() || MovingToWaypointIndex >= 0)
	{
		return;
	}

	// Move the pawn to the actor location
	// NOTE: [NWP-REVIEW] It seems that MoveToActor requires the other actor to have a collider. If the other actor does not have it, the pawn starts to move in circles.
	// That behavior does not appear using MoveToLocation.
	ANWPAIController* AIController = CastChecked<ANWPAIController>(OwnerPawn->GetController());
	AIController->MoveToLocation(WaypointsPath[WaypointIndex]->GetActorLocation(), AcceptanceRadius, true);

	// Other potential ways of moving the pawn --------------------------
	//UAITask_MoveTo::AIMoveTo(AIController, FVector::ZeroVector, GetNextWaypoint(), AcceptanceRadius);
	//UAIBlueprintHelperLibrary::SimpleMoveToActor(AIController, GetNextWaypoint());
	// --------------------------

	// Set the waypoint index that the pawn is moving to
	MovingToWaypointIndex = WaypointIndex;
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void UNWPAIPathComponent::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type MovementResult)
{
	// Only process if the waypoint behavior is active and the pawn is moving to a waypoint
	if (bIsWaypointBehaviorActive && MovingToWaypointIndex >= 0)
	{
		CurrentWaypointIndex = MovingToWaypointIndex;
		MovingToWaypointIndex = -1;
	}
}
