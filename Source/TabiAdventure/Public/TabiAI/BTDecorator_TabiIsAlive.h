// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_TabiIsAlive.generated.h"

UCLASS()
class TABIADVENTURE_API UBTDecorator_TabiIsAlive : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_TabiIsAlive();
	virtual void PostLoad() override;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
