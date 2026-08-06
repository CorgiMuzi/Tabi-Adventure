// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TabiButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTabiButtonReleasedSignature);

UCLASS()
class TABIADVENTURE_API UTabiButton : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnTabiButtonReleasedSignature OnTabiButtonReleased;

	UFUNCTION()
	void OnButtonReleased();

	UPROPERTY(EditAnywhere, Category="Tabi|Button")
	FVector2D ButtonSize{400.f, 80.f};

	UPROPERTY(EditAnywhere, Category="Tabi|Button")
	FText InButtonText;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, Category="Tabi|Button", meta=(BindWidget))
	TObjectPtr<class USizeBox> ButtonSizeBox;

	UPROPERTY(EditDefaultsOnly, Category="Tabi|Button", meta=(BindWidget))
	TObjectPtr<class UButton> TargetButton;

	UPROPERTY(EditDefaultsOnly, Category="Tabi|Button", meta=(BindWidget))
	TObjectPtr<class UTextBlock> ButtonText;
};
