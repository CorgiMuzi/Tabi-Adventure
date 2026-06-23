// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiCharacter/TabiEnemyAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "TabiCharacter/TabiEnemyBase.h"

ATabiEnemyAIController::ATabiEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;
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

	UBlackboardComponent* BB = GetBlackboardComponent();
	if (BB)
	{
		BB->SetValueAsVector(TEXT("HomeLocation"), Enemy->GetActorLocation());
		BB->SetValueAsFloat(TEXT("PatrolHalfRange"), Enemy->GetPatrolHalfRange());
		BB->SetValueAsBool(TEXT("IsAlive"), Enemy->IsAlive());
	}
}

void ATabiEnemyAIController::HandleCharacterDeath()
{
	const ATabiCharacterBase* Enemy = Cast<ATabiCharacterBase>(GetPawn());
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!BB || !Enemy) return;

	BB->SetValueAsBool(TEXT("IsAlive"), Enemy->IsAlive());
}

