// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TabiComponent/TabiVitalComponent.h"
#include "TabiVitalBar.generated.h"

UCLASS()
class TABIADVENTURE_API UTabiVitalBar : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetOwningVital(UTabiVitalComponent* InVital);

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UProgressBar> ProgressBar_Vital;

protected:
	virtual void NativeDestruct() override;

	UFUNCTION()
	void HandleVitalChanged(const ETabiVitalType& InVitalType, const float CurrentValue, const float CurrentMax);

	void UpdateVitalPercent(float VitalPercent);

	UPROPERTY()
	TWeakObjectPtr<UTabiVitalComponent> OwningVital;

	UPROPERTY(EditDefaultsOnly, Category="Tabi|UI|Vital")
	ETabiVitalType VitalType;
};
