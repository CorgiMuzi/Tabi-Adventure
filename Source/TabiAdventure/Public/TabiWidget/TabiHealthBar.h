// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TabiComponent/TabiVitalComponent.h"
#include "TabiHealthBar.generated.h"

class UProgressBar;

UCLASS()
class TABIADVENTURE_API UTabiHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetOwningVital(UTabiVitalComponent* InVital);

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Health;

protected:
	virtual void NativeDestruct() override;

	UFUNCTION()
	void HandleHealthChanged(ETabiVitalType VitalType, float CurrentValue, float CurrentMax);

	void UpdateHealthPercent(float HealthPercent);

private:
	UPROPERTY()
	TWeakObjectPtr<UTabiVitalComponent> OwningVital;
};
