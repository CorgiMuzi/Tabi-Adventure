// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiAI/BTTask_TabiMoveToPoint.h"
#include "TabiCharacter/TabiCharacterBase.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TabiMoveToPoint::UBTTask_TabiMoveToPoint()
{
	NodeName = TEXT("Move To Target");

	bNotifyTick = true;
}

void UBTTask_TabiMoveToPoint::InitializeFromAsset(UBehaviorTree& Asset)
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

EBTNodeResult::Type UBTTask_TabiMoveToPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UBTTask_TabiMoveToPoint::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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


