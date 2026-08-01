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

void UTabiVitalComponent::FillVitalValues()
{
	for (auto& Vital : Vitals)
	{
		Vital.Value.CurrentMax = Vital.Value.BaseMax;
		Vital.Value.CurrentValue = Vital.Value.BaseValue;
	}
}

void UTabiVitalComponent::SetCurrentBaseValue(const ETabiVitalType& VitalType, const float InBaseValue)
{
	if (!Vitals.Contains(VitalType)) return;

	const float NewBaseValue = FMath::Clamp(InBaseValue, 0.f, Vitals[VitalType].BaseMax);
	Vitals[VitalType].BaseValue = NewBaseValue;
	Vitals[VitalType].CurrentValue = NewBaseValue;
}

void UTabiVitalComponent::SetCurrentBaseMax(const ETabiVitalType& VitalType, const float InBaseMax)
{
	if (!Vitals.Contains(VitalType)) return;
	Vitals[VitalType].BaseMax = InBaseMax;
	Vitals[VitalType].BaseValue = FMath::Clamp(Vitals[VitalType].BaseValue, 0.f, Vitals[VitalType].BaseMax);

	Vitals[VitalType].CurrentMax = FMath::Clamp(Vitals[VitalType].CurrentMax, 0.f, Vitals[VitalType].BaseMax);
	Vitals[VitalType].CurrentValue = FMath::Clamp(Vitals[VitalType].CurrentValue, 0.f, Vitals[VitalType].CurrentMax);
}

bool UTabiVitalComponent::ModifyCurrentValue(const ETabiVitalType& VitalType, const float Amount)
{
	if (!Vitals.Contains(VitalType)) return false;

	const float NewCurrentValue = Vitals[VitalType].CurrentValue + Amount;
	Vitals[VitalType].CurrentValue = FMath::Clamp(NewCurrentValue, 0.f, Vitals[VitalType].CurrentMax);
	BroadcastVitalChanged(VitalType);
	if (Vitals[VitalType].CurrentValue <= 0.f) { BroadcastVitalDepleted(VitalType); }
	return true;
}

bool UTabiVitalComponent::ModifyCurrentMax(const ETabiVitalType& VitalType, const float Amount)
{
	if (!Vitals.Contains(VitalType)) return false;

	Vitals[VitalType].CurrentMax += Amount;
	Vitals[VitalType].CurrentValue = FMath::Clamp(Vitals[VitalType].CurrentValue, 0.f, Vitals[VitalType].CurrentMax);
	BroadcastVitalChanged(VitalType);
	return true;
}

void UTabiVitalComponent::BroadcastVitalChanged(const ETabiVitalType& VitalType) const
{
	if (!Vitals.Contains(VitalType)) return;
	const FTabiVital& Vital = Vitals[VitalType];
	OnTabiCurrentVitalChanged.Broadcast(VitalType, Vital.CurrentValue, Vital.CurrentMax);
}

void UTabiVitalComponent::BroadcastVitalDepleted(const ETabiVitalType& VitalType) const
{
	if (!Vitals.Contains(VitalType)) return;

	OnTabiVitalDepleted.Broadcast(VitalType);
}
