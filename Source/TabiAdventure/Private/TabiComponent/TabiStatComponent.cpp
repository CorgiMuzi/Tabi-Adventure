// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiComponent/TabiStatComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UTabiStatComponent::UTabiStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	InitStats();
}

void UTabiStatComponent::InitStats()
{
	for (uint8 i = 0; i < static_cast<uint8>(ETabiStatType::MAX); ++i)
	{
		const ETabiStatType Type = static_cast<ETabiStatType>(i);

		if (!Stats.Contains(Type))
		{
			Stats.Emplace(Type, FTabiStat(0.f));
		}
	}
}

void UTabiStatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		CharacterMovement = OwnerCharacter->GetCharacterMovement();
	}
}

void UTabiStatComponent::FillStatValues()
{
	for (auto& Stat : Stats)
	{
		Stat.Value.CurrentValue = Stat.Value.BaseValue;
	}
}

void UTabiStatComponent::SetStatCurrentValue(ETabiStatType StatType, float NewValue)
{
	const float OldValue = Stats[StatType].CurrentValue;
	if (FMath::IsNearlyEqual(OldValue, NewValue)) return;
	Stats[StatType].CurrentValue = NewValue;
	OnStatCurrentValueChanged.Broadcast(StatType, NewValue, OldValue);
}

void UTabiStatComponent::SetStatBaseValue(ETabiStatType StatType, float NewValue)
{
	const float OldValue = Stats[StatType].BaseValue;
	if (FMath::IsNearlyEqual(OldValue, NewValue)) return;
	Stats[StatType].BaseValue = NewValue;
	OnStatBaseValueChanged.Broadcast(StatType, NewValue, OldValue);
}

float UTabiStatComponent::GetStatCurrentValue(ETabiStatType StatType)
{
	return Stats[StatType].CurrentValue;
}

float UTabiStatComponent::GetStatBaseValue(ETabiStatType StatType)
{
	return Stats[StatType].BaseValue;
}

TMap<ETabiStatType, FTabiStat> UTabiStatComponent::GetAllStats() const
{
	return Stats;
}

