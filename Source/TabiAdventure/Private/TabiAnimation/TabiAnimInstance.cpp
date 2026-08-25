// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiAnimation/TabiAnimInstance.h"

#include "Components/AudioComponent.h"
#include "NiagaraComponent.h"
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

void UTabiAnimInstance::RegisterLoopSound(const UPaperZDAnimNotify_Base* NotifyKey, UAudioComponent* Audio)
{
	if (auto ExistSound = ActiveLoopSounds.Find(NotifyKey))
	{
		if (*ExistSound) (*ExistSound)->Stop();
	}

	ActiveLoopSounds.Add(NotifyKey, Audio);
}

void UTabiAnimInstance::StopLoopSound(const UPaperZDAnimNotify_Base* NotifyKey, const float FadeOutDuration)
{
	TObjectPtr<UAudioComponent> AudioToRemove;
	ActiveLoopSounds.RemoveAndCopyValue(NotifyKey, AudioToRemove);
	if (AudioToRemove) FadeOutDuration > 0.f ? AudioToRemove->FadeOut(FadeOutDuration, 0.f) : AudioToRemove->Stop();
}

void UTabiAnimInstance::RegisterLoopParticle(const UPaperZDAnimNotify_Base* NotifyKey, UNiagaraComponent* Particle)
{
	if (auto ExistParticle = ActiveLoopParticles.Find(NotifyKey))
	{
		if (*ExistParticle) (*ExistParticle)->Deactivate();
	}

	ActiveLoopParticles.Add(NotifyKey, Particle);
}

void UTabiAnimInstance::StopLoopParticle(const UPaperZDAnimNotify_Base* NotifyKey)
{
	TObjectPtr<UNiagaraComponent> ParticleToRemove;
	ActiveLoopParticles.RemoveAndCopyValue(NotifyKey, ParticleToRemove);
	if (ParticleToRemove) ParticleToRemove->Deactivate();
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

void UTabiAnimInstance::NotifyEnableHitCollision()
{
	OnEnableHitCollision.ExecuteIfBound();
}

void UTabiAnimInstance::NotifyDisableHitCollision()
{
	OnDisableHitCollision.ExecuteIfBound();
}

void UTabiAnimInstance::HandleAttackAnimEnd(bool IsCompleted)
{
	OnAttackAnimEnd.Broadcast(IsCompleted);
}

void UTabiAnimInstance::HandleDeadAnimEnd(bool IsCompleted)
{
	if (!IsCompleted) return;

	OnDeathAnimEnd.ExecuteIfBound();
}
