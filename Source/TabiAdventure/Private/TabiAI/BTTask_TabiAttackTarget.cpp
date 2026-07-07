// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiAI/BTTask_TabiAttackTarget.h"

#include "BehaviorTree/BehaviorTree.h"
#include "AIController.h"
#include "TabiCharacter/TabiCharacterBase.h"
#include "TabiAI/TabiAIMessages.h"

UBTTask_TabiAttackTarget::UBTTask_TabiAttackTarget()
{
	bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_TabiAttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FTabiAttackTargetMemory* MyMemory = CastInstanceNodeMemory<FTabiAttackTargetMemory>(NodeMemory);

	AAIController* MyController = OwnerComp.GetAIOwner();
	if (!MyController) return EBTNodeResult::Failed;

	ATabiCharacterBase* OwnerCharacter = MyController->GetPawn<ATabiCharacterBase>();
	if (!OwnerCharacter) return EBTNodeResult::Failed;

	const FTabiRequestID AttackRequestID = OwnerCharacter->RequestAttack();
	if (!AttackRequestID.IsValid()) return EBTNodeResult::Failed;

	MyMemory->AttackRequestID = AttackRequestID;
	WaitForMessage(OwnerComp, TabiAIMessages::AttackFinished, AttackRequestID.GetID());
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_TabiAttackTarget::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FTabiAttackTargetMemory* MyMemory = CastInstanceNodeMemory<FTabiAttackTargetMemory>(NodeMemory);

	if (MyMemory->AttackRequestID.IsValid())
	{
		AAIController* MyController = OwnerComp.GetAIOwner();
		ATabiCharacterBase* OwnerCharacter = MyController ? MyController->GetPawn<ATabiCharacterBase>() : nullptr;
		if (OwnerCharacter)
		{
			OwnerCharacter->StopAttack();
		}

		MyMemory->AttackRequestID = FTabiRequestID();
	}

	return Super::AbortTask(OwnerComp, NodeMemory);
}

uint16 UBTTask_TabiAttackTarget::GetInstanceMemorySize() const
{
	return sizeof(FTabiAttackTargetMemory);
}

void UBTTask_TabiAttackTarget::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const
{
	InitializeNodeMemory<FTabiAttackTargetMemory>(NodeMemory, InitType);
}

void UBTTask_TabiAttackTarget::CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const
{
	CleanupNodeMemory<FTabiAttackTargetMemory>(NodeMemory, CleanupType);
}
