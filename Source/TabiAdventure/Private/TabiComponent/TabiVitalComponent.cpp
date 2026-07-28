// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiComponent/TabiVitalComponent.h"

UTabiVitalComponent::UTabiVitalComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	InitVitals();
}

void UTabiVitalComponent::InitVitals()
{
	for (uint8 i = 0; i < static_cast<uint8>(ETabiVitalType::MAX); ++i)
	{
		const ETabiVitalType Type = static_cast<ETabiVitalType>(i);

		if (!Vitals.Contains(Type))
		{
			Vitals.Emplace(Type, FTabiVital(0.f, 0.f));
		}

		BroadcastVitalChanged(Type);
	}
}

void UTabiVitalComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTabiVitalComponent::FillVitalValues()
{
	for (auto& Vital : Vitals)
	{
		Vital.Value.CurrentMax = Vital.Value.BaseMax;
		Vital.Value.CurrentValue = Vital.Value.BaseValue;
	}
}

void UTabiVitalComponent::BroadcastVitalChanged(ETabiVitalType VitalType) const
{
	if (!Vitals.Contains(VitalType)) return;
	const FTabiVital& Vital = Vitals[VitalType];
	OnTabiVitalChanged.Broadcast(VitalType, Vital.CurrentValue, Vital.CurrentMax);
}

bool UTabiVitalComponent::ReceiveDamage(float Damage)
{
	if (!Vitals.Contains(ETabiVitalType::HP)) return false;

	if (!bIsVulnerable) return false;
	const float NewHP = Vitals[ETabiVitalType::HP].CurrentValue - Damage;
	Vitals[ETabiVitalType::HP].CurrentValue = FMath::Clamp(NewHP,0.f, Vitals[ETabiVitalType::HP].CurrentMax);

	BroadcastVitalChanged(ETabiVitalType::HP);
	if (FMath::IsNearlyZero(Vitals[ETabiVitalType::HP].CurrentValue)) OnCharacterDead();
	return true;
}

bool UTabiVitalComponent::ReceiveHeal(float Heal)
{
	if (!Vitals.Contains(ETabiVitalType::HP)) return false;
	const float NewHP = Vitals[ETabiVitalType::HP].CurrentValue + Heal;
	Vitals[ETabiVitalType::HP].CurrentValue = FMath::Clamp(NewHP, 0.f, Vitals[ETabiVitalType::HP].CurrentMax);
	BroadcastVitalChanged(ETabiVitalType::HP);

	return true;
}

void UTabiVitalComponent::OnCharacterDead()
{
	OnTabiHPDepleted.Broadcast();
}

void UTabiVitalComponent::SetVulnerability(bool IsVulnerable)
{
	bIsVulnerable = IsVulnerable;
}

bool UTabiVitalComponent::IsVulnerable() const
{
	return bIsVulnerable;
}
