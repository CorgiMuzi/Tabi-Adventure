
// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiWidget/TabiDefeatNotify.h"
#include "TabiSubsystem/TabiGameFlowSubsystem.h"

#include "TabiWidget/TabiButton.h"

void UTabiDefeatNotify::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Retry->OnTabiButtonReleased.AddDynamic(this, &ThisClass::OnRetryButtonReleased);
	Button_MainMenu->OnTabiButtonReleased.AddDynamic(this, &ThisClass::OnMainMenuButtonReleased);
}

void UTabiDefeatNotify::NativeDestruct()
{
	Button_Retry->OnTabiButtonReleased.RemoveDynamic(this, &ThisClass::OnRetryButtonReleased);
	Button_MainMenu->OnTabiButtonReleased.RemoveDynamic(this, &ThisClass::OnMainMenuButtonReleased);

	Super::NativeDestruct();
}

void UTabiDefeatNotify::OnRetryButtonReleased()
{
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UTabiGameFlowSubsystem* Subsystem = GI->GetSubsystem<UTabiGameFlowSubsystem>())
		{
			Subsystem->RestartCurrentGame();
		}
	}
}

void UTabiDefeatNotify::OnMainMenuButtonReleased()
{
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UTabiGameFlowSubsystem* Subsystem = GI->GetSubsystem<UTabiGameFlowSubsystem>())
		{
			Subsystem->ReturnToMainMenu();
		}
	}
}
