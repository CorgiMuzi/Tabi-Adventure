// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiAnimation/TabiAnimInstance.h"
#include "TabiData/TabiAttackDefinition.h"

UTabiAnimInstance::UTabiAnimInstance()
{
}

void UTabiAnimInstance::OnInit_Implementation()
{
	Super::OnInit_Implementation();
}

void UTabiAnimInstance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool UTabiAnimInstance::PlayAttackAnimation(const UPaperZDAnimSequence* AttackAnimSequence)
{
	if (!AttackAnimSequence) UE_LOG(LogTemp, Warning, TEXT("Attack animation is not set."));
	return PlayAnimationOverride(AttackAnimSequence, TEXT("DefaultSlot"), 1.f, 0.f, FZDOnAnimationOverrideEndSignature::CreateUObject(this, &ThisClass::HandleAttackAnimEnd));
}

void UTabiAnimInstance::StopAttackAnimation()
{
	StopAnimationOverrideByGroup(TEXT("DefaultGroup"));
}

bool UTabiAnimInstance::PlayDeadAnimation(const UPaperZDAnimSequence* DeadAnimSequence)
{
	if(!DeadAnimSequence) UE_LOG(LogTemp, Warning, TEXT("Dead animtion is not set."));
	return PlayAnimationOverride(DeadAnimSequence, TEXT("DefaultSlot"), 1.f, 0, FZDOnAnimationOverrideEndSignature::CreateUObject(this, &ThisClass::HandleDeadAnimEnd));
}

void UTabiAnimInstance::PlayNotify_EnableHitCollision()
{
	OnEnableHitCollision.Execute();
}

void UTabiAnimInstance::PlayNotify_DisableHitCollision()
{
	OnDisableHitCollision.Execute();
}

void UTabiAnimInstance::HandleAttackAnimEnd(bool IsCompleted)
{
	if (!IsCompleted)
	{
		OnDisableHitCollision.Execute();
	}

	OnAttackAnimEnd.Broadcast(IsCompleted);
}

void UTabiAnimInstance::HandleDeadAnimEnd(bool IsCompleted)
{
	if (!IsCompleted) return;

	OnDeathAnimEnd.Execute();
}
