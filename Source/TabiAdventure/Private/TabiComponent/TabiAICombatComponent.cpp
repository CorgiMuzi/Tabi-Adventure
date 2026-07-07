// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiComponent/TabiAICombatComponent.h"

UTabiAICombatComponent::UTabiAICombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTabiAICombatComponent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
