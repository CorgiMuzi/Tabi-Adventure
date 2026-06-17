// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TabiStatComponent.generated.h"

UENUM(BlueprintType)
enum class ETabiStatType : uint8
{
	Strength UMETA(DisplayName="Strength"),
	Defense UMETA(DisplayName="Defense"),
	Agility UMETA(DisplayName="Agility"),
	Intelligence UMETA(DisplayName="Intelligence"),
	Speed UMETA(DisplayName="Speed"),

	Max UMETA(DisplayName="Max")
};

USTRUCT(BlueprintType)
struct FTabiStat
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Tabi|Stat")
	float BaseValue{0.f};

	UPROPERTY(BlueprintReadOnly, Category="Tabi|Stat")
	float CurrentValue{0.f};

	FTabiStat() = default;
	FTabiStat(float InitValue) : BaseValue(InitValue), CurrentValue(InitValue) {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStatChanged, ETabiStatType, StateType, const float, NewStat, const float, OldStat);

UCLASS(ClassGroup=(Tabi), meta=(BlueprintSpawnableComponent))
class TABIADVENTURE_API UTabiStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTabiStatComponent();

	virtual void BeginPlay() override;

	//~ Stat Delegates
	FOnStatChanged OnStatCurrentValueChanged;
	FOnStatChanged OnStatBaseValueChanged;
	//~ End Stat Delegates

protected:
	UPROPERTY(EditDefaultsOnly, Category="Tabi|Stat")
	TMap<ETabiStatType, FTabiStat> Stats;

private:
	void InitStat();

public:
	void SetStatCurrentValue(ETabiStatType StatType, float NewValue);
	void SetStatBaseValue(ETabiStatType StatType, float NewValue);
	float GetStatCurrentValue(ETabiStatType StatType);
	float GetStatBaseValue(ETabiStatType StatType);
};
