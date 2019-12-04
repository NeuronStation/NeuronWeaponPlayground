// Copyright 2019 Neuron Station. All Rights Reserved.

// Class
#include "NWPAIController.h"

// NWP
#include "NeuronWeaponPlayground.h"
#include "NWPCharacter.h"
#include "NWPAIConfigurationComponent.h"

// UE
#include "Kismet/KismetSystemLibrary.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"

// Log category
DEFINE_LOG_CATEGORY(LogNWPAIController);

// Constants
const FName ANWPAIController::IsFollowerLabel(TEXT("IsFollower"));
const FName ANWPAIController::HasSightLabel(TEXT("HasSight"));
const FName ANWPAIController::TargetActorLabel(TEXT("TargetActor"));

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

ANWPAIController::ANWPAIController(const class FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Cache the current owner
	check(InPawn);
	check(!CurrentOwner);
	CurrentOwner = CastChecked<ANWPCharacter>(InPawn);

	// Get the ai configuration component
	UNWPAIConfigurationComponent* AIConfigurationComponent = Cast<UNWPAIConfigurationComponent>(
		CurrentOwner->GetComponentByClass(UNWPAIConfigurationComponent::StaticClass()));

	// Use the behavior tree of the ai configuration component or the default behavior tree of the ai controller
	if (AIConfigurationComponent && AIConfigurationComponent->GetConfiguredBehaviorTree())
	{
		StartBehaviouTree(AIConfigurationComponent->GetConfiguredBehaviorTree());
		BuildBlackboard(AIConfigurationComponent);
	}
	else if (DefaultBehaviorTree)
	{
		StartBehaviouTree(DefaultBehaviorTree);
	}
	else
	{
		V_LOG(LogNWPAIController, Warning, TEXT("There is no valid behaviour tree"));
	}

	// Register to the target perception updated callback
	if (PerceptionComponent)
	{
		PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ANWPAIController::OnTargetPerceptionUpdated);
	}
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPAIController::OnUnPossess()
{
	// Unregister from the target perception updated callback
	if (PerceptionComponent)
	{
		PerceptionComponent->OnTargetPerceptionUpdated.RemoveDynamic(this, &ANWPAIController::OnTargetPerceptionUpdated);
	}

	// Stop the current behavior tree & clean the current owner
	StopBehaviorTree();
	CurrentOwner = nullptr;

	Super::OnUnPossess();
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPAIController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	V_LOG(LogNWPAIController, Warning, TEXT("The team id cannot be set directly"));
	return;
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

FGenericTeamId ANWPAIController::GetGenericTeamId() const
{
	// Get the team id from the owner if valid
	const IGenericTeamAgentInterface* CurrentOwnerAgent = Cast<const IGenericTeamAgentInterface>(CurrentOwner);

	if (CurrentOwnerAgent)
	{
		return CurrentOwnerAgent->GetGenericTeamId();
	}

	return FGenericTeamId::NoTeam;
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

ETeamAttitude::Type ANWPAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* CurrentOwnerAgent = Cast<const IGenericTeamAgentInterface>(CurrentOwner);
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(&Other);
	return OtherTeamAgent ? FGenericTeamId::GetAttitude(CurrentOwnerAgent->GetGenericTeamId(), OtherTeamAgent->GetGenericTeamId())
		: ETeamAttitude::Neutral;
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPAIController::StartBehaviouTree(class UBehaviorTree* _BehaviorTreeToStart)
{
	// Check if the behavior tree is different & valid
	if (CurrentBehaviorTree == _BehaviorTreeToStart || !_BehaviorTreeToStart)
	{
		return;
	}

	// Run & cache the behavior tree
	RunBehaviorTree(_BehaviorTreeToStart);
	CurrentBehaviorTree = _BehaviorTreeToStart;
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPAIController::StopBehaviorTree()
{
	// Stop the behavior tree if the is one running
	if (CurrentBehaviorTree)
	{
		UBehaviorTreeComponent* BehaviorTreeComponent = CastChecked<UBehaviorTreeComponent>(BrainComponent);
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPAIController::BuildBlackboard(class UNWPAIConfigurationComponent* _AIConfigurationComponent)
{
	UpdateIsFollowerKey(_AIConfigurationComponent->IsFollower());
	UpdateHasSightKey(false);
	UpdateTargetActorKey(nullptr);
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPAIController::UpdateIsFollowerKey(bool _bIsFollower)
{
	if (Blackboard)
	{
		Blackboard->SetValueAsBool(IsFollowerLabel, _bIsFollower);
	}
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPAIController::UpdateHasSightKey(bool _bHasSight)
{
	if (Blackboard)
	{
		Blackboard->SetValueAsBool(HasSightLabel, _bHasSight);
	}
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPAIController::UpdateTargetActorKey(AActor* _TargetActor)
{
	if (Blackboard)
	{
		Blackboard->SetValueAsObject(TargetActorLabel, _TargetActor);
	}
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

void ANWPAIController::OnTargetPerceptionUpdated(AActor* _StimulusCauser, FAIStimulus _Stimulus)
{
	// Check if the stimulus was successfully sensed
	if (!_Stimulus.WasSuccessfullySensed())
	{
		return;
	}

	// TODO: [NWP-REVIEW] Check why if i place the macro Text() around the string literal, the code does not compile
	V_LOGM(LogNWPAIController, Log, "Character %s has perceived %s", *UKismetSystemLibrary::GetDisplayName(CurrentOwner),
		*UKismetSystemLibrary::GetDisplayName(_StimulusCauser));

	// Check if the perceived actor is a character
	if (_StimulusCauser->IsA<ANWPCharacter>())
	{
		UpdateHasSightKey(true);
		UpdateTargetActorKey(_StimulusCauser);
	}
}
