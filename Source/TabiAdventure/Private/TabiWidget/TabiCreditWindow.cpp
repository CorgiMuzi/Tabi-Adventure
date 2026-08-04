// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiWidget/TabiCreditWindow.h"

#include "Components/PanelWidget.h"
#include "TabiSubsystem/TabiGameFlowSubsystem.h"

void UTabiCreditWindow::NativeConstruct()
{
	Super::NativeConstruct();

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(CreditHoldTimerHandle, this, &ThisClass::StartCreditScrolling, InitialHoldTime, false);
	}
}

void UTabiCreditWindow::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsCreditScrolling)
	{
		CreditElapsedTime += InDeltaTime;
		const float Progress = FMath::Clamp(CreditElapsedTime / CreditDuration, 0.f, 1.f);
		const float CurveValue = ScrollProgressCurve ? ScrollProgressCurve->GetFloatValue(Progress) : Progress;
		const float TotalCreditLength = CreditLength  + MyGeometry.GetLocalSize().Y;
		const float YOffset = CurveValue * TotalCreditLength;
		CreditContainer->SetRenderTranslation(FVector2D(0.f, -YOffset));

		if (Progress >= 1.f)
		{
			if (UGameInstance* GI = GetGameInstance())
			{
				if (UTabiGameFlowSubsystem* Subsystem = GI->GetSubsystem<UTabiGameFlowSubsystem>())
				{
					Subsystem->ReturnToMainMenu();
				}
			}
		}
	}
}

void UTabiCreditWindow::StartCreditScrolling()
{
	if (!CreditContainer) return;

	CreditLength = CreditContainer->GetDesiredSize().Y;
	UE_LOG(LogTemp, Verbose, TEXT("CreditLenght: %f"), CreditLength);
	if (CreditLength <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Credit Length is 0 - Unable to scroll the credit container."));
		return;
	}

	CreditDuration = CreditLength / FMath::Max(ScrollSpeed, 1.f);

	bIsCreditScrolling = true;
}
