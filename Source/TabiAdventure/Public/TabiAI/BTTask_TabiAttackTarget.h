// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TabiAttackTarget.generated.h"

/**
 * 
 */
UCLASS()
class TABIADVENTURE_API UBTTask_TabiAttackTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_TabiAttackTarget();

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
};
