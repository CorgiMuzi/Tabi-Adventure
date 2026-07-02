// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TabiPickPatrolPoint.generated.h"

struct FTabiPickPatrolTargetMemory
{
	bool bGoingRight = true;
};

UCLASS()
class TABIADVENTURE_API UBTTask_TabiPickPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_TabiPickPatrolPoint();
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual uint16 GetInstanceMemorySize() const override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(VisibleAnywhere, Category="Tabi|AI")
	FBlackboardKeySelector PatrolPointKey;

	UPROPERTY(VisibleAnywhere, Category="Tabi|AI")
	FBlackboardKeySelector PatrolHalfRangeKey;
};
