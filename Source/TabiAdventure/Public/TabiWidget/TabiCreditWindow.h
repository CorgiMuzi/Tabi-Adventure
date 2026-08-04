// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TabiCreditWindow.generated.h"

/**
 * 
 */
UCLASS()
class TABIADVENTURE_API UTabiCreditWindow : public UUserWidget
{
	GENERATED_BODY()


protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void StartCreditScrolling();

	UPROPERTY(EditDefaultsOnly, Category="Tabi|Credit")
	TObjectPtr<UCurveFloat> ScrollProgressCurve;

	UPROPERTY(EditAnywhere, Category="Tabi|Credit")
	float InitialHoldTime = 1.f;

	UPROPERTY(EditAnywhere, Category="Tabi|Credit")
	float ScrollSpeed;

	float CreditDuration;
	float CreditElapsedTime;

	UPROPERTY(EditDefaultsOnly, Category="Tabi|Credit", meta=(BindWidget))
	TObjectPtr<class UPanelWidget> CreditContainer;

	UPROPERTY()
	float CreditLength = 1.f;

	bool bIsCreditScrolling{false};
	FTimerHandle CreditHoldTimerHandle;
};
