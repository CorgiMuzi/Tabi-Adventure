// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TabiCreditWindow.generated.h"

class UVerticalBox;
class UTextBlock;
class UTabiCreditData;
class UTabiCreditSection;

UCLASS()
class TABIADVENTURE_API UTabiCreditWindow : public UUserWidget
{
	GENERATED_BODY()


protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void BuildCredits();

	UFUNCTION(BlueprintCallable, Category="Tabi|Credit")
	bool StartCreditScrolling();
	void ScrollCreditContainer(const FGeometry& MyGeometry, float InDeltaTime);
	void ScrollCreditComment(const FGeometry& MyGeometry, float InDeltaTime);
	void LeaveCredits();

	UPROPERTY(EditDefaultsOnly, Category="Tabi|Credit")
	TObjectPtr<UTabiCreditData> CreditData;

	UPROPERTY(EditDefaultsOnly, Category="Tabi|Credit")
	TSubclassOf<UTabiCreditSection> CreditSectionClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Tabi|Credit")
	TSubclassOf<UUserWidget> CreditTitleCardClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Tabi|Credit")
	float TitleSpacing = 120.f;

	UPROPERTY(EditDefaultsOnly, Category="Tabi|Credit")
	float SectionSpacing = 60.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Tabi|Credit")
	float InitialHoldTime = 2.5f;

	UPROPERTY(EditDefaultsOnly, Category="Tabi|Credit")
	float FinalHoldTime = 1.f;

	UPROPERTY(EditDefaultsOnly, Category="Tabi|Credit")
	float InitializeTimeout = 3.f;

	UPROPERTY(EditAnywhere, Category="Tabi|Credit")
	float ScrollSpeed = 120.f;
	
	float ViewportHeight{0.f};

	float ContainerYOffset{0.f};
	float CommentYOffset{0.f};

	UPROPERTY(EditDefaultsOnly, Category="Tabi|Credit", meta=(BindWidget))
	TObjectPtr<UVerticalBox> CreditContainer;

	UPROPERTY(EditDefaultsOnly, Category="Tabi|Credit", meta=(BindWidget))
	TObjectPtr<UTextBlock> CreditComment;
	
	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> TitleCardWidget;

	bool bIsContainerScrolling{false};
	bool bIsCommentScrolling{false};

	FTimerHandle InitialHoldTimerHandle;
	FTimerHandle FinalHoldTimerHandle;

private:
	bool bIsInitialized{false};
	bool bIsLeaving{false};
	float InitializeElapsed{0.f};
};
