// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiAnimation/TabiAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "TabiCharacter/TabiCharacterBase.h"
#include "TabiData/TabiAttackDefinition.h"

UTabiAnimInstance::UTabiAnimInstance()
{
}

void UTabiAnimInstance::OnInit_Implementation()
{
	Super::OnInit_Implementation();

	OwningCharacter = Cast<ATabiCharacterBase>(GetOwningActor());
	if (OwningCharacter)
	{
		CharacterMovement = OwningCharacter->GetCharacterMovement();
	}
}

void UTabiAnimInstance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OwningCharacter)
	{
		Speed = FMath::Abs(OwningCharacter->GetVelocity().Size2D());
	}

	if (CharacterMovement)
	{
		bIsFalling = CharacterMovement->IsFalling();
	}
}

bool UTabiAnimInstance::PlayAttackAnimation(UTabiAttackDefinition* AttackDef)
{
	if (!AttackDef || !AttackDef->AnimSequence) return false;
	return PlayAnimationOverride(AttackDef->AnimSequence, TEXT("DefaultSlot"), 1.f, 0.f, FZDOnAnimationOverrideEndSignature::CreateUObject(this, &ThisClass::HandleAttackEnd));
}

void UTabiAnimInstance::PlayNotify_EnableHitCollision()
{
	OnEnableHitCollision.Execute();
}

void UTabiAnimInstance::PlayNotify_DisableHitCollision()
{
	OnDisableHitCollision.Execute();
}

void UTabiAnimInstance::HandleAttackEnd(bool bIsCompleted)
{
	if (!bIsCompleted) return;

	OnAttackAnimEnd.Execute();
}
