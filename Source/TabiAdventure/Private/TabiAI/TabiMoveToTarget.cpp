// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiAI/TabiMoveToTarget.h"
#include "TabiCharacter/TabiCharacterBase.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

UTabiMoveToTarget::UTabiMoveToTarget()
{
	NodeName = TEXT("Move To Target");
}

void UTabiMoveToTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	bNotifyTick = true;

	if (UBlackboardData* BBAsset = Asset.GetBlackboardAsset())
	{
		TargetLocationKey.ResolveSelectedKey(*BBAsset);
	}
}

EBTNodeResult::Type UTabiMoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UTabiMoveToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	FVector TargetLocation = BB->GetValueAsVector(TargetLocationKey.SelectedKeyName);

	APawn* Owner = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	if (!Owner) return FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	const float DiffX = TargetLocation.X - Owner->GetActorLocation().X;

	if (FMath::Abs(DiffX) < RadiusTolerance) return FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	if (ATabiCharacterBase* OwnerCharacter = Cast<ATabiCharacterBase>(Owner))
	{
		OwnerCharacter->MoveAlongX(FMath::Sign(DiffX));
	}
}


