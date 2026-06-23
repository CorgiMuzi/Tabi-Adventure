// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TabiMoveToTarget.generated.h"

struct FTargetLocationToMove
{
	FVector TargetLocationToMove;
};

UCLASS()
class TABIADVENTURE_API UTabiMoveToTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UTabiMoveToTarget();
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual auto TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) -> void override;

protected:
	UPROPERTY(EditAnywhere, Category="Tabi|AI")
	FBlackboardKeySelector TargetLocationKey;

	UPROPERTY(EditAnywhere, Category="Tabi|AI")
	float RadiusTolerance;
};
