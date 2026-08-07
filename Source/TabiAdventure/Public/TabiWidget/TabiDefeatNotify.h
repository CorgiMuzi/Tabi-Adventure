// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TabiDefeatNotify.generated.h"

class UTabiButton;
class UButton;

UCLASS()
class TABIADVENTURE_API UTabiDefeatNotify : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnRetryButtonReleased();

	UFUNCTION()
	void OnMainMenuButtonReleased();

	UFUNCTION()
	void OnQuitButtonReleased();

	UPROPERTY(EditDefaultsOnly, Category="Tabi|Widget", meta=(BindWidget))
	TObjectPtr<UTabiButton> Button_Retry;

	UPROPERTY(EditDefaultsOnly, Category="Tabi|Widget", meta=(BindWidget))
	TObjectPtr<UTabiButton> Button_MainMenu;

	UPROPERTY(EditDefaultsOnly, Category="Tabi|Widget", meta=(BindWidget))
	TObjectPtr<UTabiButton> Button_Quit;
};
