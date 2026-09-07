// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiWidget/TabiCreditWindow.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/TextBlock.h"
#include "TabiData/TabiCreditData.h"
#include "TabiSubsystem/TabiGameFlowSubsystem.h"
#include "TabiWidget/TabiCreditSection.h"

namespace
{
	float GetArrangedHeight(const UWidget& InWidget)
	{
		return FMath::Max(InWidget.GetCachedGeometry().GetLocalSize().Y, InWidget.GetDesiredSize().Y);
	}

	/**
	 * Distance from the top of InRoot down to InWidget's bottom edge, in InRoot's local space.
	 *
	 * Read from the arranged tree rather than derived from the canvas anchor: Slate folds a widget's
	 * render transform into the geometry its parent arranges for it, so this follows the scroll
	 * instead of describing where the widget would sit at rest. Anchors, slot padding, and whether the
	 * window actually fills the viewport all stop mattering, because none of them are assumed.
	 *
	 * The geometry is one frame stale, which at scroll speed is a pixel or two of overshoot.
	 */
	float GetLocalBottomEdge(const FGeometry& InRootGeometry, const UWidget& InWidget)
	{
		const FGeometry& WidgetGeometry = InWidget.GetCachedGeometry();
		const FVector2D BottomEdge(0.f, GetArrangedHeight(InWidget));

		return InRootGeometry.AbsoluteToLocal(WidgetGeometry.LocalToAbsolute(BottomEdge)).Y;
	}

	bool HasBeenArranged(const UWidget& InWidget)
	{
		return InWidget.GetCachedGeometry().GetLocalSize().Y > 0.f;
	}
}

void UTabiCreditWindow::NativeConstruct()
{
	Super::NativeConstruct();

	BuildCredits();
}

void UTabiCreditWindow::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bIsInitialized)
	{
		bIsInitialized = StartCreditScrolling();
		if (!bIsInitialized)
		{
			InitializeElapsed += InDeltaTime;
			if (InitializeElapsed >= InitializeTimeout)
			{
				UE_LOG(LogTemp, Warning,
				       TEXT("TabiCreditWindow: layout never produced a usable size, leaving the credits."));
				LeaveCredits();
			}
			return;
		}
	}

	if (bIsContainerScrolling) ScrollCreditContainer(MyGeometry, InDeltaTime);
	if (bIsCommentScrolling) ScrollCreditComment(MyGeometry, InDeltaTime);
}

void UTabiCreditWindow::BuildCredits()
{
	if (!CreditContainer) return;

	CreditContainer->ClearChildren();
	TitleCardWidget = nullptr;

	if (!ensure(CreditData) || !ensure(CreditSectionClass)) return;

	if (CreditComment)
	{
		CreditComment->SetText(CreditData->ClosingComment);
	}

	if (CreditTitleCardClass)
	{
		TitleCardWidget = CreateWidget<UUserWidget>(this, CreditTitleCardClass);
		if (TitleCardWidget)
		{
			if (UVerticalBoxSlot* TitleSlot = CreditContainer->AddChildToVerticalBox(TitleCardWidget))
			{
				TitleSlot->SetPadding(FMargin(0.f, 0.f, 0.f, TitleSpacing));
			}
		}
	}

	for (const FTabiCreditSectionData& Section : CreditData->Sections)
	{
		UTabiCreditSection* NewSection = CreateWidget<UTabiCreditSection>(this, CreditSectionClass);
		if (!NewSection) continue;

		NewSection->SetSection(Section);

		if (UVerticalBoxSlot* SectionSlot = CreditContainer->AddChildToVerticalBox(NewSection))
		{
			SectionSlot->SetPadding(FMargin(0.f, 0.f, 0.f, SectionSpacing));
		}
	}
}

bool UTabiCreditWindow::StartCreditScrolling()
{
	if (!CreditContainer || !CreditComment) return false;

	CreditContainer->ForceLayoutPrepass();
	CreditComment->ForceLayoutPrepass();

	ViewportHeight = UWidgetLayoutLibrary::GetViewportWidgetGeometry(this).GetLocalSize().Y;
	if (ViewportHeight <= 0.f) return false;

	if (CreditContainer->GetDesiredSize().Y <= 0.f) return false;

	// The roll starts with its top edge on the viewport's bottom edge. Skipping ahead by half the
	// viewport plus half the title card puts the title card's centre on the screen's centre, which
	// is where it sits for InitialHoldTime before the roll starts moving. Same form as
	// CommentScrollTarget, because it is the same problem: centre one element on screen.
	// A title card that measures zero is a legitimate state - an empty image brush, or no title card
	// configured at all. Treat it as "no title to centre", never as a failure to initialise.
	ContainerYOffset = 0.f;
	if (TitleCardWidget)
	{
		const float TitleHeight = TitleCardWidget->GetDesiredSize().Y;
		if (TitleHeight > 0.f)
		{
			ContainerYOffset = (ViewportHeight + TitleHeight) * 0.5f;
		}
	}
	CreditContainer->SetRenderTranslation(FVector2D(0.f, -ContainerYOffset));

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(InitialHoldTimerHandle, [this]()
		{
			bIsContainerScrolling = bIsCommentScrolling = true;
		}, InitialHoldTime, false);
	}

	return true;
}

void UTabiCreditWindow::ScrollCreditContainer(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (!CreditContainer) return;
	ContainerYOffset += ScrollSpeed * InDeltaTime;
	CreditContainer->SetRenderTranslation(FVector2D(0.f, -ContainerYOffset));

	// The roll is done once its bottom edge has passed the top of the window. Falls back to the
	// distance the roll was assumed to need only while the container has never been arranged.
	const bool bIsCleared = HasBeenArranged(*CreditContainer)
		                        ? GetLocalBottomEdge(MyGeometry, *CreditContainer) <= 0.f
		                        : ContainerYOffset >= ViewportHeight + CreditContainer->GetDesiredSize().Y;

	if (bIsCleared)
	{
		bIsContainerScrolling = false;
	}
}

void UTabiCreditWindow::ScrollCreditComment(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (!CreditComment) return;
	if (bIsContainerScrolling) return;
	CommentYOffset += ScrollSpeed * InDeltaTime;
	CreditComment->SetRenderTranslation(FVector2D(0.f, -CommentYOffset));

	// Settles once the comment's own centre reaches the centre of the window. Same measurement as the
	// container, offset by half the comment so the line stops centred rather than fully cleared.
	const bool bHasSettled = HasBeenArranged(*CreditComment)
		                         ? GetLocalBottomEdge(MyGeometry, *CreditComment)
		                           - GetArrangedHeight(*CreditComment) * 0.5f
		                           <= MyGeometry.GetLocalSize().Y * 0.5f
		                         : CommentYOffset >= (ViewportHeight + CreditComment->GetDesiredSize().Y) * 0.5f;

	if (bHasSettled)
	{
		bIsCommentScrolling = false;
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(FinalHoldTimerHandle, [this]()
			{
				LeaveCredits();
			}, FinalHoldTime, false);
		}
		return;
	}
}

void UTabiCreditWindow::LeaveCredits()
{
	if (bIsLeaving) return;
	bIsLeaving = true;

	if (UGameInstance* GI = GetGameInstance())
	{
		if (UTabiGameFlowSubsystem* Subsystem = GI->GetSubsystem<UTabiGameFlowSubsystem>())
		{
			Subsystem->ReturnToMainMenu();
		}
	}
}
