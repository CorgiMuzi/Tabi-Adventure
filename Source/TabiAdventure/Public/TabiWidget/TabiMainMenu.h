// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TabiMainMenu.generated.h"

class UTabiButton;

UCLASS()
class TABIADVENTURE_API UTabiMainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnStartButtonReleased();

	UFUNCTION()
	void OnQuitButtonReleased();

	UPROPERTY(EditDefaultsOnly, Category = "Tabi|MainMenu", meta=(BindWidget))
	TObjectPtr<UTabiButton> Button_Start;

	UPROPERTY(EditDefaultsOnly, Category = "Tabi|MainMenu", meta=(BindWidget))
	TObjectPtr<UTabiButton> Button_Quit;
};
