// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiWidget/TabiPlayerHUD.h"
#include "TabiComponent/TabiVitalComponent.h"
#include "TabiWidget/TabiHealthBar.h"

void UTabiPlayerHUD::SetOwningVital(UTabiVitalComponent* InVital)
{
	if (HealthBar) HealthBar->SetOwningVital(InVital);
}
