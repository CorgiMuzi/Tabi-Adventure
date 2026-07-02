// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiAI/UBTTask_TabiMoveToPoint.h"
#include "TabiCharacter/TabiCharacterBase.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

UUBTTask_TabiMoveToPoint::UUBTTask_TabiMoveToPoint()
{
	NodeName = TEXT("Move To Target");

	bNotifyTick = true;
}

void UUBTTask_TabiMoveToPoint::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBAsset = Asset.GetBlackboardAsset())
	{
		PatrolPointKey.ResolveSelectedKey(*BBAsset);
	}else
	{
		PatrolPointKey.InvalidateResolvedKey();
	}
}

EBTNodeResult::Type UUBTTask_TabiMoveToPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UUBTTask_TabiMoveToPoint::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	FVector TargetLocation = BB->GetValueAsVector(PatrolPointKey.SelectedKeyName);

	APawn* Owner = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	if (!Owner) return FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	const float XDiff = TargetLocation.X - Owner->GetActorLocation().X;

	if (FMath::Abs(XDiff) < RadiusTolerance) return FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	ATabiCharacterBase* OwnerCharacter = Cast<ATabiCharacterBase>(Owner);
	if (!OwnerCharacter) return FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	OwnerCharacter->MoveAlongX(FMath::Sign(XDiff));
	return FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
}


