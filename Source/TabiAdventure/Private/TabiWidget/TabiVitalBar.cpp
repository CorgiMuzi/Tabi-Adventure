// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiWidget/TabiVitalBar.h"
#include "Components/ProgressBar.h"

void UTabiVitalBar::SetOwningVital(UTabiVitalComponent* InVital)
{
	if (InVital == nullptr) return;

	if (OwningVital.IsValid())
	{
		OwningVital->OnTabiCurrentVitalChanged.RemoveDynamic(this, &ThisClass::HandleVitalChanged);
	}

	OwningVital = InVital;
	OwningVital->OnTabiCurrentVitalChanged.AddDynamic(this, &ThisClass::HandleVitalChanged);

	const float CurrentValue = OwningVital->GetCurrentValueByType(VitalType);
	const float CurrentMax = OwningVital->GetCurrentMaxByType(VitalType);
	HandleVitalChanged(VitalType, CurrentValue, CurrentMax);
}

void UTabiVitalBar::HandleVitalChanged(const ETabiVitalType& InVitalType, const float CurrentValue, const float CurrentMax)
{
	if (VitalType != InVitalType) return;

	if (CurrentMax <= 0.f) return UpdateVitalPercent(0.f);

	const float VitalPercent = FMath::Clamp(CurrentValue / CurrentMax, 0.f, 1.f);
	UpdateVitalPercent(VitalPercent);
}

void UTabiVitalBar::UpdateVitalPercent(float VitalPercent)
{
	if (!ProgressBar_Vital) return;
	ProgressBar_Vital->SetPercent(VitalPercent);
}

void UTabiVitalBar::NativeDestruct()
{
	if (OwningVital.IsValid())
	{
		OwningVital->OnTabiCurrentVitalChanged.RemoveDynamic(this, &ThisClass::HandleVitalChanged);
	}

	Super::NativeDestruct();
}
