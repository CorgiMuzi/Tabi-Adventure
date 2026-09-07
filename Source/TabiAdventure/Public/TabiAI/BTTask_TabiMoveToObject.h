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

	/**
	 * Used only when the owner has no attack definition to derive a band from.
	 * Normally the stop band comes from MinAttackRange / MaxAttackRange on the attack
	 * definitions, so this node and the attack decorator can never disagree.
	 */
	UPROPERTY(EditAnywhere, Category="Tabi|AI", meta=(ClampMin="0"))
	float FallbackStopDistance = 60.f;

	/** Extra margin kept inside the attack band so the enemy does not stop right on the edge. */
	UPROPERTY(EditAnywhere, Category="Tabi|AI", meta=(ClampMin="0"))
	float BandMargin = 10.f;
};
