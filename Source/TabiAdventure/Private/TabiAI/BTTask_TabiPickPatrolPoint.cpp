// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiAI/BTTask_TabiPickPatrolPoint.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TabiPickPatrolPoint::UBTTask_TabiPickPatrolPoint()
{
	NodeName = TEXT("Pick Patrol Point");
}

void UBTTask_TabiPickPatrolPoint::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (const UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		PatrolPointKey.ResolveSelectedKey(*BBAsset);
		PatrolHalfRangeKey.ResolveSelectedKey(*BBAsset);
	}else
	{
		PatrolPointKey.InvalidateResolvedKey();
		PatrolHalfRangeKey.InvalidateResolvedKey();
	}
}

uint16 UBTTask_TabiPickPatrolPoint::GetInstanceMemorySize() const
{
	return sizeof(FTabiPickPatrolTargetMemory);
}

EBTNodeResult::Type UBTTask_TabiPickPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	BB->SetValueAsVector(PatrolPointKey.SelectedKeyName, TargetLocation);

	Memory->bGoingRight = !Memory->bGoingRight;

	return EBTNodeResult::Succeeded;
}
