// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiAI/BTTask_TabiAttackTarget.h"

#include "BehaviorTree/BehaviorTree.h"
#include "AIController.h"
#include "TabiCharacter/TabiEnemyBase.h"

UBTTask_TabiAttackTarget::UBTTask_TabiAttackTarget()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_TabiAttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* OwnerPawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	if (!OwnerPawn) return EBTNodeResult::Failed;
	ATabiEnemyBase* OwnerCharacter = Cast<ATabiEnemyBase>(OwnerPawn);
	if (!OwnerCharacter) return EBTNodeResult::Failed;

	OwnerCharacter->HandleAttackInput();

	return EBTNodeResult::InProgress;
}

void UBTTask_TabiAttackTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	APawn* Owner = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	const ATabiCharacterBase* OwnerCharacter = Owner ? Cast<ATabiCharacterBase>(Owner) : nullptr;
	if (!OwnerCharacter) return FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	const EBTNodeResult::Type CurrentNodeResult = OwnerCharacter->GetCharacterState() != ETabiCharacterState::Attacking ? EBTNodeResult::Succeeded : EBTNodeResult::InProgress;
	FinishLatentTask(OwnerComp, CurrentNodeResult);
}
