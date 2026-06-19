// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiComponent/TabiStatComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UTabiStatComponent::UTabiStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTabiStatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		CharacterMovement = OwnerCharacter->GetCharacterMovement();
	}
}

void UTabiStatComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UTabiStatComponent::InitStat()
{
	for (uint8 i = 0; i < static_cast<uint8>(ETabiStatType::MAX); ++i)
	{
		const ETabiStatType Type = static_cast<ETabiStatType>(i);

		if (!Stats.Contains(Type))
		{
			Stats.Emplace(Type, FTabiStat(0.f));
			continue;
		}

		Stats[Type].CurrentValue = Stats[Type].BaseValue;
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

