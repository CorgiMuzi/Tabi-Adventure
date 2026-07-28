// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TabiPlayerHUD.generated.h"

class UTabiVitalComponent;
class UTabiHealthBar;

UCLASS()
class TABIADVENTURE_API UTabiPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetOwningVital(UTabiVitalComponent* InVital);

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTabiHealthBar> HealthBar;
};
