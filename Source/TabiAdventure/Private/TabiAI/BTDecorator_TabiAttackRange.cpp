// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#include "TabiAI/BTDecorator_TabiAttackRange.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "TabiCharacter/TabiEnemyBase.h"
#include "AIController.h"
#include "TabiConstant/TabiEnemyBlackboardKeys.h"

UBTDecorator_TabiAttackRange::UBTDecorator_TabiAttackRange()
{
	NodeName = TEXT("Is Close To Attack");
	INIT_DECORATOR_NODE_NOTIFY_FLAGS();
}

void UBTDecorator_TabiAttackRange::PostLoad()
{
	Super::PostLoad();

	bNotifyTick = (FlowAbortMode != EBTFlowAbortMode::None);
}

bool UBTDecorator_TabiAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* OwnerPawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	if (!OwnerPawn) return false;

	const ATabiEnemyBase* OwnerCharacter = Cast<ATabiEnemyBase>(OwnerPawn);
	if (!OwnerCharacter) return false;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return false;

	const AActor* Target = Cast<AActor>(BB->GetValueAsObject(TabiEnemyBlackboardKey::Target));

	float DistanceToTarget = TNumericLimits<float>::Max();
	if (Target)
	{
		const float XDiff = Target->GetActorLocation().X - OwnerCharacter->GetActorLocation().X;
		DistanceToTarget = FMath::Abs(XDiff);
	}

	const float AttackRadius = OwnerCharacter->GetAttackRadius();
	/********************************************************
	 *	CharSizeX : CharacterHalfSize().X
	 *
	 * <--                     Hit Box                    -->
	 * <--          Hurt Box          -->
	 * <-- CharSizeX --><-- CharSizeX --><-- AttackRadius -->
	 *                  <--          Is In Range          -->
	 *******************************************************/
	const bool IsInRange = DistanceToTarget <= OwnerCharacter->GetCharacterHalfSize().X + AttackRadius;

	return IsInRange && OwnerCharacter->IsOnSamePlatformAs(Target);
}

void UBTDecorator_TabiAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ConditionalFlowAbort(OwnerComp, EBTDecoratorAbortRequest::ConditionResultChanged);
}

