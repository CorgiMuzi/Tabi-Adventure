// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_TabiAttackRange.generated.h"

UCLASS()
class TABIADVENTURE_API UBTDecorator_TabiAttackRange : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_TabiAttackRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
