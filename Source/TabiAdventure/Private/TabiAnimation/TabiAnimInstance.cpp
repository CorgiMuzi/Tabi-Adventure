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

bool UTabiAnimInstance::PlayAttackAnimation(UTabiAttackDefinition* AttackDef)
{
	if (!AttackDef || !AttackDef->AnimSequence) return false;
	return PlayAnimationOverride(AttackDef->AnimSequence, TEXT("DefaultSlot"), 1.f, 0.f, FZDOnAnimationOverrideEndSignature::CreateUObject(this, &ThisClass::HandleAttackAnimEnd));
}

bool UTabiAnimInstance::PlayDeadAnimation()
{
	if (!DeadAnimSequence) return false;
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

void UTabiAnimInstance::HandleAttackAnimEnd(bool bIsCompleted)
{
	if (!bIsCompleted)
	{
		OnDisableHitCollision.Execute();
		return;
	}

	OnAttackAnimEnd.Execute();
}

void UTabiAnimInstance::HandleDeadAnimEnd(bool bIsCompleted)
{
	if (!bIsCompleted) return;

	OnDeathAnimEnd.Execute();
}
