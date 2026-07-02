// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UBTTask_TabiMoveToPoint.generated.h"

UCLASS()
class TABIADVENTURE_API UUBTTask_TabiMoveToPoint : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UUBTTask_TabiMoveToPoint();
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;

	UPROPERTY(VisibleAnywhere, Category="Tabi|AI")
	FBlackboardKeySelector PatrolPointKey;

	UPROPERTY(EditAnywhere, Category="Tabi|AI")
	float RadiusTolerance;
};
