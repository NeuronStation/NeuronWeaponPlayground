// Copyright 2019 Neuron Station. All Rights Reserved.

#pragma once

// UE
#include "Perception/AIPerceptionTypes.h"

// Generated
#include "CoreMinimal.h"
#include "AIController.h"
#include "NWPAIController.generated.h"

// Log category
DECLARE_LOG_CATEGORY_EXTERN(LogNWPAIController, Log, All);

/**
 * The AI controller that controls the character.
 */
UCLASS()
class NEURONWEAPONPLAYGROUND_API ANWPAIController : public AAIController
{
	GENERATED_BODY()

// Constants
public:

	static const FName IsFollowerLabel;
	static const FName HasSightLabel;
	static const FName TargetActorLabel;

// Constructors
public:
	ANWPAIController(const class FObjectInitializer& ObjectInitializer);

// Member functions
public:

	// Starts a behavior tree (checking if it is compatible with it) on the ai controller
	UFUNCTION(BlueprintCallable, Category = "AI")
	void StartCharacterBehaviorTree(class UBehaviorTree* _CharacterBehaviorTreeToStart);

	// Stops the behavior tree currently being executed on the ai controller
	UFUNCTION(BlueprintCallable, Category = "AI")
	void StopCharacterBehaviorTree();

protected:

	/// AController interface begin
	// Handles attaching this controller to the specified pawn
	virtual void OnPossess(APawn* InPawn) override;

	// Called to unpossess our pawn for any reason that is not the pawn being destroyed (destruction handled by PawnDestroyed())
	virtual void OnUnPossess() override;
	/// AController interface end

	/// IGenericTeamAgentInterface interface begin
	// Assigns Team Agent to given TeamID
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;

	// Retrieve team identifier in form of FGenericTeamId
	virtual FGenericTeamId GetGenericTeamId() const override;

	// Retrieved owner attitude toward given Other object
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	/// IGenericTeamAgentInterface interface end

	// Starts a behavior tree on the ai controller
	void StartBehaviouTree(class UBehaviorTree* _BehaviorTreeToStart);

	// Stops the behavior tree currently being executed on the ai controller
	void StopBehaviorTree();

	// Builds a blackborad using the ai configuration
	void BuildBlackboard(class UNWPAIConfigurationComponent* _AIConfigurationComponent);

	// Updates the follower key from the blackboard
	void UpdateIsFollowerKey(bool _bIsFollower);

	// Updates the has sight key from the blackboard
	void UpdateHasSightKey(bool _bHasSight);

	// Updates the target actor key from the blackboard
	void UpdateTargetActorKey(AActor* _TargetActor);

	// Callback executed when a stimulus is perceived by the perception component
	UFUNCTION(Category = "Perception")
	void OnTargetPerceptionUpdated(AActor* _StimulusCauser, FAIStimulus _Stimulus);

// Member variables
protected:

	// Behavior tree used if no behavior tree configured on a ai configuration component 
	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* DefaultBehaviorTree;

	// Current behavior tree being executed
	UPROPERTY(Transient, SkipSerialization)
	class UBehaviorTree* CurrentBehaviorTree;

	// Current owner of the ai controller
	UPROPERTY(Transient, SkipSerialization)
	class ANWPCharacter* CurrentOwner;
};
