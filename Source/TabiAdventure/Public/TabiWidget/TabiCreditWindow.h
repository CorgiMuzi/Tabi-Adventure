// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TabiCreditWindow.generated.h"

class UVerticalBox;
class UTextBlock;

UCLASS()
class TABIADVENTURE_API UTabiCreditWindow : public UUserWidget
{
	GENERATED_BODY()


protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Tabi|Credit")
	void StartCreditScrolling();
	void ScrollCreditContainer(float InDeltaTime);
	void ScrollCreditComment(float InDeltaTime);

	UPROPERTY(EditDefaultsOnly, Category="Tabi|Credit")
	float InitialHoldTime = 0.3f;

	UPROPERTY(EditDefaultsOnly, Category="Tabi|Credit")
	float FinalHoldTime = 1.f;

	UPROPERTY(EditAnywhere, Category="Tabi|Credit")
	float ScrollSpeed;

	float ContainerHeight;
	float ContainerScrollTarget;
	float ContainerYOffset{0.f};

	float CommentHeight;
	float CommentScrollTarget;
	float CommentYOffset{0.f};

	UPROPERTY(EditDefaultsOnly, Category="Tabi|Credit", meta=(BindWidget))
	TObjectPtr<UVerticalBox> CreditContainer;

	UPROPERTY(EditDefaultsOnly, Category="Tabi|Credit", meta=(BindWidget))
	TObjectPtr<UTextBlock> CreditComment;

	bool bIsContainerScrolling{false};
	bool bIsCommentScrolling{false};

	FTimerHandle InitialHoldTimerHandle;
	FTimerHandle FinalHoldTimerHandle;

private:
	bool bIsInitialized{false};
};
