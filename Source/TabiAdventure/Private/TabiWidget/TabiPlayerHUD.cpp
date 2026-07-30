// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiWidget/TabiPlayerHUD.h"
#include "TabiWidget/TabiVitalBar.h"

void UTabiPlayerHUD::SetOwningVital(UTabiVitalComponent* InVital)
{
	if (HealthBar) HealthBar->SetOwningVital(InVital);
}
