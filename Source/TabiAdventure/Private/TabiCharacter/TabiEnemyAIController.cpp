// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiCharacter/TabiEnemyAIController.h"

#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TabiGameFramework/TabiEnemyBlackboardKeys.h"
#include "TabiCharacter/TabiEnemyBase.h"
#include "TabiComponent/TabiCombatComponent.h"
#include "TabiAI/TabiAIMessages.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ATabiEnemyAIController::ATabiEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = false;

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("SightConfig");

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ThisClass::HandleTargetPerceptionUpdated);
}

void ATabiEnemyAIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SightConfig->SightRadius = SightRadius;
	SightConfig->LoseSightRadius = LoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = PeripheralVisionHalfAngle;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetDominantSense(*SightConfig->GetSenseImplementation());
	PerceptionComponent->SetSenseEnabled(UAISense_Sight::StaticClass(), true);

	PerceptionComponent->RequestStimuliListenerUpdate();
}

void ATabiEnemyAIController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (ATabiEnemyBase* OwningPawn = GetPawn<ATabiEnemyBase>())
	{
		OwningPawn->SetGenericTeamId(NewTeamID);
	}

	Super::SetGenericTeamId(NewTeamID);
}

FGenericTeamId ATabiEnemyAIController::GetGenericTeamId() const
{
	if (const ATabiEnemyBase* OwningPawn = GetPawn<ATabiEnemyBase>())
	{
		return OwningPawn->GetGenericTeamId();
	}

	return Super::GetGenericTeamId();
}

void ATabiEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATabiEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ATabiEnemyBase* OwningCharacter = CastChecked<ATabiEnemyBase>(InPawn);
	OwningCharacter->OnTabiCharacterDead.AddDynamic(this, &ThisClass::HandleCharacterDeath);

	if (UTabiCombatComponent* CombatComp = OwningCharacter->GetCombatComponent())
	{
		ObservedCombatComponent = CombatComp;
		AttackEndHandle = CombatComp->OnTabiAttackEnd.AddUObject(this, &ThisClass::HandleAttackEnd);
	}

	UBehaviorTree* BT = OwningCharacter->GetBehaviorTree();
	RunBehaviorTree(BT);

	if (UBlackboardComponent* BB = GetBlackboardComponent())
	{
		BB->SetValueAsFloat(TabiEnemyBlackboardKey::PatrolHalfRange, OwningCharacter->GetPatrolHalfRange());
		BB->SetValueAsBool(TabiEnemyBlackboardKey::IsAlive, OwningCharacter->IsAlive());
	}
}

void ATabiEnemyAIController::OnUnPossess()
{
	if (ObservedCombatComponent.IsValid())
	{
		ObservedCombatComponent->OnTabiAttackEnd.Remove(AttackEndHandle);
	}

	ObservedCombatComponent.Reset();
	AttackEndHandle.Reset();

	if (ATabiCharacterBase* OwningCharacter = GetPawn<ATabiCharacterBase>())
	{
		OwningCharacter->OnTabiCharacterDead.RemoveDynamic(this, &ThisClass::ATabiEnemyAIController::HandleCharacterDeath);
	}

	Super::OnUnPossess();
}

void ATabiEnemyAIController::HandleAttackEnd(const FTabiRequestID RequestID, bool IsSucceeded)
{
	if (!RequestID.IsValid()) return;

	const FAIMessage AttackFinish(TabiAIMessages::AttackFinished, this, RequestID.GetID(), IsSucceeded);
	FAIMessage::Send(this, AttackFinish);
}

void ATabiEnemyAIController::HandleCharacterDeath()
{
	const ATabiCharacterBase* Enemy = Cast<ATabiCharacterBase>(GetPawn());
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!BB || !Enemy) return;

	BB->SetValueAsBool(TabiEnemyBlackboardKey::IsAlive, Enemy->IsAlive());

	PerceptionComponent->SetSenseEnabled(UAISense_Sight::StaticClass(), false);
}

void ATabiEnemyAIController::HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Stimulus.WasSuccessfullySensed())
	{
		UpdateTarget(Actor, false);
		return;
	}

	ATabiCharacterBase* Target = Cast<ATabiCharacterBase>(Actor);
	if (!Target) return;

	UpdateTarget(Target, true);
}

void ATabiEnemyAIController::UpdateTarget(AActor* Target, bool IsSensed)
{
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!BB) return;

	if (IsSensed)
	{
		const IGenericTeamAgentInterface* TargetTeam = Cast<IGenericTeamAgentInterface>(Target);
		if (!TargetTeam || FGenericTeamId::GetAttitude(GetGenericTeamId(), TargetTeam->GetGenericTeamId()) != ETeamAttitude::Hostile) return;
	}

	ATabiCharacterBase* LastTarget = Cast<ATabiCharacterBase>(BB->GetValueAsObject(TabiEnemyBlackboardKey::Target));

	// If enemy has no target
	if (!LastTarget && IsSensed)
	{
   		BB->SetValueAsObject(TabiEnemyBlackboardKey::Target, Target);
		return;
	}

	// If the sensed target is already in pursuit.
	if (Target == LastTarget)
	{
		if (IsSensed) return;
		BB->SetValueAsObject(TabiEnemyBlackboardKey::Target, nullptr);
		return;
	}

	// else the new target has been sensed
	// check the priority between new and old target
	// BB->SetValueAsObject(TabiEnemyBlackboardKey::TargetToAttack, CheckPriority(Target, LastTarget));
}

