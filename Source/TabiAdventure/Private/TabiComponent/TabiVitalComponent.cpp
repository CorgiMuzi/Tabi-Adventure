// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiComponent/TabiVitalComponent.h"

UTabiVitalComponent::UTabiVitalComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	InitVitals();
}

void UTabiVitalComponent::BeginPlay()
{
	Super::BeginPlay();

	FillVitalValues();
}

void UTabiVitalComponent::ReceiveDamage(float Damage)
{
	if (!Vitals.Contains(ETabiVitalType::HP)) return;
	Vitals[ETabiVitalType::HP].CurrentValue -= Damage;
}

void UTabiVitalComponent::ReceiveHeal(float Heal)
{
	if (!Vitals.Contains(ETabiVitalType::HP)) return;
	Vitals[ETabiVitalType::HP].CurrentValue += Heal;
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
	}
}

void UTabiVitalComponent::FillVitalValues()
{
	for (auto& Vital : Vitals)
	{
		Vital.Value.CurrentMax = Vital.Value.BaseMax;
		Vital.Value.CurrentValue = Vital.Value.BaseValue;
	}
}

