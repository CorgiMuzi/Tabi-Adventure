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

	ATabiCharacterBase* OwnerCharacter = Cast<ATabiCharacterBase>(Owner);
	if (!OwnerCharacter) return FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	const FVector& TargetLocation = Target->GetActorLocation();
	const float XDiff = TargetLocation.X - Owner->GetActorLocation().X;
	const float Distance = FMath::Abs(XDiff);

	float MinRange = 0.f;
	float MaxRange = FallbackStopDistance;
	if (!OwnerCharacter->GetAttackDistanceBand(Target, MinRange, MaxRange))
	{
		MinRange = 0.f;
		MaxRange = FallbackStopDistance;
	}
	
	float ApproachTarget = FMath::Clamp(MaxRange - BandMargin, MinRange, MaxRange);
	float RetreatTarget = MinRange > 0.f ? FMath::Clamp(MinRange + BandMargin, MinRange, MaxRange) : 0.f;
	
	if (ApproachTarget < RetreatTarget)
	{
		const float Midpoint = (MinRange + MaxRange) * 0.5f;
		ApproachTarget = Midpoint;
		RetreatTarget = Midpoint;
	}

	// Direction toward the target. Sign() returns 0 when perfectly aligned, so fall back to the facing.
	float TowardTarget = FMath::Sign(XDiff);
	if (FMath::IsNearlyZero(TowardTarget)) TowardTarget = OwnerCharacter->IsFacingRight() ? 1.f : -1.f;

	if (Distance > ApproachTarget)
	{
		OwnerCharacter->MoveAlongX(TowardTarget);
		return;
	}

	if (MinRange > 0.f && Distance < RetreatTarget)
	{
		OwnerCharacter->MoveAlongX(-TowardTarget);
		OwnerCharacter->FaceToward(Target);
		return;
	}
	
	OwnerCharacter->FaceToward(Target);
	return FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
