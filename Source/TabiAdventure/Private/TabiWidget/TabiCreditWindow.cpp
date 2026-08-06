// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiWidget/TabiCreditWindow.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "TabiSubsystem/TabiGameFlowSubsystem.h"

void UTabiCreditWindow::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTabiCreditWindow::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bIsInitialized) StartCreditScrolling();

	if (bIsContainerScrolling) ScrollCreditContainer(InDeltaTime);
	if (bIsCommentScrolling) ScrollCreditComment(InDeltaTime);
}

void UTabiCreditWindow::StartCreditScrolling()
{
	if (!CreditContainer || !CreditComment) return;

	const float ViewportHeight = UWidgetLayoutLibrary::GetViewportWidgetGeometry(this).GetLocalSize().Y;

	ContainerHeight = CreditContainer->GetDesiredSize().Y;
	CommentHeight = CreditComment->GetDesiredSize().Y;

	ContainerScrollTarget = ViewportHeight + ContainerHeight;
	CommentScrollTarget = ViewportHeight / 2.f + CommentHeight * 0.5f;

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(InitialHoldTimerHandle, [this]()
		{
			bIsContainerScrolling = bIsCommentScrolling = true;
		}, InitialHoldTime, false);
	}

	bIsInitialized = true;
}

void UTabiCreditWindow::ScrollCreditContainer(float InDeltaTime)
{
	if (!CreditContainer) return;
	ContainerYOffset += ScrollSpeed * InDeltaTime;
	CreditContainer->SetRenderTranslation(FVector2D(0.f, -ContainerYOffset));

	if (ContainerYOffset >= ContainerScrollTarget)
	{
		bIsContainerScrolling = false;
		return;
	}
}

void UTabiCreditWindow::ScrollCreditComment(float InDeltaTime)
{
	if (!CreditComment) return;
	if (bIsContainerScrolling) return;
	CommentYOffset += ScrollSpeed * InDeltaTime;
	CreditComment->SetRenderTranslation(FVector2D(0.f, -CommentYOffset));

	if (CommentYOffset >= CommentScrollTarget)
	{
		bIsCommentScrolling = false;
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(FinalHoldTimerHandle, [this]()
			{
				if (UGameInstance* GI = GetGameInstance())
				{
					if (UTabiGameFlowSubsystem* Subsystem = GI->GetSubsystem<UTabiGameFlowSubsystem>())
					{
						Subsystem->ReturnToMainMenu();
					}
				}
			}, FinalHoldTime, false);
		}
		return;
	}
}
