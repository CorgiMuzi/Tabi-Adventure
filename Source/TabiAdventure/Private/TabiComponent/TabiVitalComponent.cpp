// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiComponent/TabiVitalComponent.h"

UTabiVitalComponent::UTabiVitalComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTabiVitalComponent::BeginPlay()
{
	Super::BeginPlay();

	InitVitals();

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("HP: %f / %f"), Vitals[ETabiVitalType::HP].CurrentValue, Vitals[ETabiVitalType::HP].CurrentMax));
}

void UTabiVitalComponent::InitVitals()
{
	for (uint8 i = 0; i < static_cast<uint8>(ETabiVitalType::Max); ++i)
	{
		const ETabiVitalType Type = static_cast<ETabiVitalType>(i);

		if (!Vitals.Contains(Type))
		{
			Vitals.Emplace(Type, FTabiVital(0.f, 0.f));
			continue;
		}

		Vitals[Type].CurrentMax = Vitals[Type].BaseMax;
		Vitals[Type].CurrentValue = Vitals[Type].BaseValue;
	}
}

