// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiComponent/TabiCombatVFXComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "TabiComponent/TabiCombatComponent.h"
#include "TabiData/TabiAttackDefinition.h"

UTabiCombatVFXComponent::UTabiCombatVFXComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTabiCombatVFXComponent::BeginPlay()
{
	Super::BeginPlay();

	const AActor* Owner = GetOwner();
	if (!Owner) return;

	CombatComponent = Owner->FindComponentByClass<UTabiCombatComponent>();
	if (CombatComponent.IsValid())
	{
		HitConfirmedHandle = CombatComponent->OnTabiHitConfirmed.AddUObject(this, &ThisClass::HandleHitConfirmed);
	}
}

void UTabiCombatVFXComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CombatComponent.IsValid() && HitConfirmedHandle.IsValid())
	{
		CombatComponent->OnTabiHitConfirmed.Remove(HitConfirmedHandle);
	}
	HitConfirmedHandle.Reset();

	Super::EndPlay(EndPlayReason);
}

void UTabiCombatVFXComponent::HandleHitConfirmed(const FTabiHitEvent& HitEvent)
{
	switch (HitEvent.Result)
	{
		case ETabiHitResult::Damaged: {
			if (!HitEvent.bFirstConfirmedHit) break;

			UNiagaraSystem* Effect = HitEvent.AttackDefinition ? HitEvent.AttackDefinition->GetHitEffect() : nullptr;
			FVector Scale = FVector::OneVector;

			if (Effect)
			{
				Scale = HitEvent.AttackDefinition->GetHitEffectScale();
			}
			else
			{
				Effect = DefaultHitEffect;
			}

			SpawnOneShotEffect(Effect, HitEvent.Location, HitEvent.Rotation, Scale);
			break;
		}

		case ETabiHitResult::Evaded: {
			const AActor* Victim = HitEvent.Victim;
			if (!Victim) break;

			if (const UTabiCombatVFXComponent* VictimVFX = Victim->FindComponentByClass<UTabiCombatVFXComponent>())
			{
				VictimVFX->PlayEvadeEffect(HitEvent.Location, HitEvent.Rotation);
			}
			break;
		}

		default:
			break;
	}
}

void UTabiCombatVFXComponent::PlayEvadeEffect(const FVector& Location, const FRotator& Rotation) const
{
	SpawnOneShotEffect(EvadeEffect, Location, Rotation, EvadeEffectScale);
}

void UTabiCombatVFXComponent::SpawnOneShotEffect(UNiagaraSystem* Effect, const FVector& Location, const FRotator& Rotation, const FVector& Scale) const
{
	if (!Effect) return;

	UWorld* World = GetWorld();
	if (!World) return;

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, Effect, Location, Rotation, Scale, true);
}
