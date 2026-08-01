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

	MAX UMETA(Hidden)
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTabiVitalDepletedSignature, const ETabiVitalType&, InVitalType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTabiCurrentVitalChangedSignature, const ETabiVitalType&, InVitalType, const float, CurrentValue, const float, CurrentMax);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTabiBaseVitalChangedSignature, const ETabiVitalType&, InVitalType, const float, BaseValue, const float, BaseMax);

UCLASS(ClassGroup=(Tabi), meta=(BlueprintSpawnableComponent))
class TABIADVENTURE_API UTabiVitalComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTabiVitalComponent();

	virtual void BeginPlay() override;
	void FillVitalValues();

	void SetCurrentBaseValue(const ETabiVitalType& VitalType, const float InBaseValue);
	void SetCurrentBaseMax(const ETabiVitalType& VitalType, const float InBaseMax);
	bool ModifyCurrentValue(const ETabiVitalType& VitalType, const float Amount);
	bool ModifyCurrentMax(const ETabiVitalType& VitalType, const float Amount);

	FOnTabiVitalDepletedSignature OnTabiVitalDepleted;
	FOnTabiCurrentVitalChangedSignature OnTabiCurrentVitalChanged;
	FOnTabiBaseVitalChangedSignature OnTabiBaseVitalChanged;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Tabi|Vital")
	TMap<ETabiVitalType, FTabiVital> Vitals;

private:
	void InitVitals();
	void BroadcastVitalChanged(const ETabiVitalType& VitalType) const;
	void BroadcastVitalDepleted(const ETabiVitalType& VitalType) const;

public:
	float GetCurrentValueByType(ETabiVitalType VitalType) const { return Vitals[VitalType].CurrentValue; }
	float GetCurrentMaxByType(ETabiVitalType VitalType) const { return Vitals[VitalType].CurrentMax; }
};

