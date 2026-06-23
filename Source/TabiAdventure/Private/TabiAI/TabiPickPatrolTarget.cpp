// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiAI/TabiPickPatrolTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UTabiPickPatrolTarget::UTabiPickPatrolTarget()
{
	NodeName = TEXT("Pick Patrol Target");
}

void UTabiPickPatrolTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (const UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		TargetLocationKey.ResolveSelectedKey(*BBAsset);
		PatrolHalfRangeKey.ResolveSelectedKey(*BBAsset);
	}else
	{
		TargetLocationKey.InvalidateResolvedKey();
		PatrolHalfRangeKey.InvalidateResolvedKey();
	}
}

uint16 UTabiPickPatrolTarget::GetInstanceMemorySize() const
{
	return sizeof(FTabiPickPatrolTargetMemory);
}

EBTNodeResult::Type UTabiPickPatrolTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return EBTNodeResult::Failed;

	APawn* AIPawn = AICon->GetPawn();
	if (!AIPawn) return EBTNodeResult::Failed;

	const FVector LastLocation = AIPawn->GetActorLocation();

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;

	FTabiPickPatrolTargetMemory* Memory = CastInstanceNodeMemory<FTabiPickPatrolTargetMemory>(NodeMemory);

	const float PatrolHalfRange = BB->GetValueAsFloat(PatrolHalfRangeKey.SelectedKeyName);
	const FVector TargetLocation = LastLocation + FVector(Memory->bGoingRight ? PatrolHalfRange : -PatrolHalfRange, 0.f, 0.f);
	BB->SetValueAsVector(TargetLocationKey.SelectedKeyName, TargetLocation);

	Memory->bGoingRight = !Memory->bGoingRight;

	return EBTNodeResult::Succeeded;
}
