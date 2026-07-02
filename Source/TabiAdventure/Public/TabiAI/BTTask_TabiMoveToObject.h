// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TabiMoveToObject.generated.h"

UCLASS()
class TABIADVENTURE_API UBTTask_TabiMoveToObject : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_TabiMoveToObject();

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, Category="Tabi|AI")
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, Category="Tabi|AI")
	float DistanceTolerance;
};
