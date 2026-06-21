// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TabiVitalComponent.generated.h"

UENUM(BlueprintType)
enum class ETabiVitalType : uint8
{
	HP UMETA(DisplayName="HP"),
	MP UMETA(DisplayName="MP"),
	Stamina UMETA(DisplayName="Stamina"),

	MAX UMETA(DisplayName="MAX")
};

USTRUCT(BlueprintType)
struct FTabiVital
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Tabi|Vital", meta=(ClampMin="0"))
	float BaseValue = 0.f;

	UPROPERTY(EditDefaultsOnly, Category="Tabi|Vital", meta=(ClampMin="0"))
	float BaseMax = 0.f;

	UPROPERTY(BlueprintReadOnly, Category="Tabi|Vital", meta=(ClampMin="0"))
	float CurrentValue = 0.f;

	UPROPERTY(BlueprintReadOnly, Category="Tabi|Vital", meta=(ClampMin="0"))
	float CurrentMax = 0.f;

	FTabiVital() = default;
	FTabiVital(float InitValue, float InitMax) : BaseValue{InitValue} , BaseMax{InitMax}, CurrentValue{InitValue}, CurrentMax{InitMax} {}
};

DECLARE_DYNAMIC_DELEGATE(FOnTabiCharacterDeadSignature);

UCLASS(ClassGroup=(Tabi), meta=(BlueprintSpawnableComponent))
class TABIADVENTURE_API UTabiVitalComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTabiVitalComponent();
	virtual void BeginPlay() override;

	bool ReceiveDamage(float Damage);
	bool ReceiveHeal(float Heal);

	FOnTabiCharacterDeadSignature OnTabiCharacterDead;


protected:
	void OnCharacterDead();

	UPROPERTY(EditDefaultsOnly, Category="Tabi|Vital")
	TMap<ETabiVitalType, FTabiVital> Vitals;

private:
	void InitVitals();
	void FillVitalValues();

public:
	bool IsInVulnerable() const;

	float GetCurrentValueByType(ETabiVitalType VitalType) const { return Vitals[VitalType].CurrentValue; }
	float GetCurrentMaxByType(ETabiVitalType VitalType) const { return Vitals[VitalType].CurrentMax; }
};

