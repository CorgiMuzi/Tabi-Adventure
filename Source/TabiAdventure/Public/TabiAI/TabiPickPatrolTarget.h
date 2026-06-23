// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TabiPickPatrolTarget.generated.h"

struct FTabiPickPatrolTargetMemory
{
	bool bGoingRight = true;
};

UCLASS()
class TABIADVENTURE_API UTabiPickPatrolTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UTabiPickPatrolTarget();
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual uint16 GetInstanceMemorySize() const;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	/// Target location will be removed when AI can move based on the direction not to the target point.
	UPROPERTY(VisibleAnywhere, Category="Tabi|AI")
	FBlackboardKeySelector TargetLocationKey;

	UPROPERTY(VisibleAnywhere, Category="Tabi|AI")
	FBlackboardKeySelector PatrolHalfRangeKey;
};
