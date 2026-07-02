// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiCharacter/TabiEnemyAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "TabiGameFramework/TabiEnemyBlackboardKeys.h"
#include "TabiCharacter/TabiEnemyBase.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ATabiEnemyAIController::ATabiEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = false;

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerceptionComponent");
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("SightConfig");
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ThisClass::HandleTargetPerceptionUpdated);
}

void ATabiEnemyAIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SightConfig->SightRadius = SightRadius;
	SightConfig->LoseSightRadius = LoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = PeripheralVisionHalfAngle;
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(*SightConfig->GetSenseImplementation());
	AIPerceptionComponent->SetSenseEnabled(UAISense_Sight::StaticClass(), true);
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

	ATabiEnemyBase* Enemy = CastChecked<ATabiEnemyBase>(InPawn);
	Enemy->OnTabiCharacterDead.AddDynamic(this, &ThisClass::HandleCharacterDeath);

	UBehaviorTree* BT = Enemy->GetBehaviorTree();
	RunBehaviorTree(BT);

	if (UBlackboardComponent* BB = GetBlackboardComponent())
	{
		BB->SetValueAsFloat(TabiEnemyBlackboardKey::PatrolHalfRange, Enemy->GetPatrolHalfRange());
		BB->SetValueAsBool(TabiEnemyBlackboardKey::IsAlive, Enemy->IsAlive());
	}
}

void ATabiEnemyAIController::HandleCharacterDeath()
{
	const ATabiCharacterBase* Enemy = Cast<ATabiCharacterBase>(GetPawn());
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!BB || !Enemy) return;

	BB->SetValueAsBool(TabiEnemyBlackboardKey::IsAlive, Enemy->IsAlive());
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

	AActor* LastTarget = Cast<AActor>(BB->GetValueAsObject(TabiEnemyBlackboardKey::Target));

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

