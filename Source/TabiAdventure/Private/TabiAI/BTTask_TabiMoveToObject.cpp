// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiAI/BTTask_TabiMoveToObject.h"

#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TabiCharacter/TabiCharacterBase.h"

UBTTask_TabiMoveToObject::UBTTask_TabiMoveToObject()
{
	bNotifyTick = true;
}

void UBTTask_TabiMoveToObject::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBAsset = Asset.GetBlackboardAsset())
	{
		TargetKey.ResolveSelectedKey(*BBAsset);
	}else
	{
		TargetKey.InvalidateResolvedKey();
	}
}

EBTNodeResult::Type UBTTask_TabiMoveToObject::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UBTTask_TabiMoveToObject::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	APawn* Owner = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	if (!Owner) return FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	const AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!Target) return FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	const FVector& TargetLocation = Target->GetActorLocation();
	const float XDiff = TargetLocation.X - Owner->GetActorLocation().X;

	if (FMath::Abs(XDiff) < DistanceTolerance) return FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	ATabiCharacterBase* OwnerCharacter = Cast<ATabiCharacterBase>(Owner);
	if (!OwnerCharacter) return FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	OwnerCharacter->MoveAlongX(FMath::Sign(XDiff));
	return FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
}
