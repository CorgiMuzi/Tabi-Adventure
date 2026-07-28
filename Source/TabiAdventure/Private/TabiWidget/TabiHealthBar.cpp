// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiWidget/TabiHealthBar.h"
#include "Components/ProgressBar.h"

void UTabiHealthBar::SetOwningVital(UTabiVitalComponent* InVital)
{
	if (InVital == nullptr) return;

	if (OwningVital.IsValid())
	{
		OwningVital->OnTabiVitalChanged.RemoveDynamic(this, &ThisClass::HandleHealthChanged);
	}

	OwningVital = InVital;
	OwningVital->OnTabiVitalChanged.AddDynamic(this, &ThisClass::HandleHealthChanged);

	// Pull values when binding for the first time to prevent missing the timing.
	const float CurrentHP = OwningVital->GetCurrentValueByType(ETabiVitalType::HP);
	const float CurrentMax = OwningVital->GetCurrentMaxByType(ETabiVitalType::HP);
	HandleHealthChanged(ETabiVitalType::HP, CurrentHP, CurrentMax);
}

void UTabiHealthBar::NativeDestruct()
{
	if (OwningVital.IsValid())
	{
		OwningVital->OnTabiVitalChanged.RemoveDynamic(this, &ThisClass::HandleHealthChanged);
	}

	Super::NativeDestruct();
}

void UTabiHealthBar::HandleHealthChanged(ETabiVitalType VitalType, float CurrentValue, float CurrentMax)
{
	if (VitalType != ETabiVitalType::HP) return;

	if (CurrentMax <= 0.f) return UpdateHealthPercent(0.f);
	const float HealthRatio = FMath::Clamp(CurrentValue / CurrentMax, 0.f, 1.f);
	UpdateHealthPercent(CurrentValue / CurrentMax);
}

void UTabiHealthBar::UpdateHealthPercent(float HealthPercent)
{
	if (ProgressBar_Health == nullptr) return;
	ProgressBar_Health->SetPercent(HealthPercent);
}
