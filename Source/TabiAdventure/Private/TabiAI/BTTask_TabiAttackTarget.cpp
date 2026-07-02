// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiAI/BTTask_TabiAttackTarget.h"

#include "BehaviorTree/BehaviorTree.h"
#include "AIController.h"
#include "TabiCharacter/TabiEnemyBase.h"

UBTTask_TabiAttackTarget::UBTTask_TabiAttackTarget()
{
}

void UBTTask_TabiAttackTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
}

EBTNodeResult::Type UBTTask_TabiAttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* OwnerPawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	if (!OwnerPawn) return EBTNodeResult::Failed;
	ATabiEnemyBase* OwnerCharacter = Cast<ATabiEnemyBase>(OwnerPawn);
	if (!OwnerCharacter) return EBTNodeResult::Failed;

	OwnerCharacter->Attack();

	return EBTNodeResult::Succeeded;
}
