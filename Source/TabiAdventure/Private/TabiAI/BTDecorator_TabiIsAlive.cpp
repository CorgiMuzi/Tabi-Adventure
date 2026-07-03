// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiAI/BTDecorator_TabiIsAlive.h"

#include "TabiCharacter/TabiCharacterBase.h"

#include "AIController.h"

UBTDecorator_TabiIsAlive::UBTDecorator_TabiIsAlive()
{
	INIT_DECORATOR_NODE_NOTIFY_FLAGS();
}

void UBTDecorator_TabiIsAlive::PostLoad()
{
	Super::PostLoad();

	bNotifyTick = (FlowAbortMode != EBTFlowAbortMode::None);
}

bool UBTDecorator_TabiIsAlive::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ATabiCharacterBase* OwnerCharacter = OwnerComp.GetAIOwner() ?
		OwnerComp.GetAIOwner()->GetPawn<ATabiCharacterBase>() :
		nullptr;

	if (!OwnerCharacter) return false;

	return OwnerCharacter->IsAlive();
}

void UBTDecorator_TabiIsAlive::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ConditionalFlowAbort(OwnerComp, EBTDecoratorAbortRequest::ConditionResultChanged);
}
