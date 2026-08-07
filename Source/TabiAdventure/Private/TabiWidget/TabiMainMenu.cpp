// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiWidget/TabiMainMenu.h"

#include "TabiSubsystem/TabiGameFlowSubsystem.h"
#include "TabiWidget/TabiButton.h"

void UTabiMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Start->OnTabiButtonReleased.AddDynamic(this, &ThisClass::OnStartButtonReleased);
	Button_Quit->OnTabiButtonReleased.AddDynamic(this, &ThisClass::OnQuitButtonReleased);

	if (APlayerController* PC = GetOwningPlayer())
	{
		FInputModeUIOnly UIInputMode;
		PC->SetInputMode(UIInputMode);
		PC->SetShowMouseCursor(true);
	}
}

void UTabiMainMenu::NativeDestruct()
{
	Button_Start->OnTabiButtonReleased.RemoveDynamic(this, &ThisClass::OnStartButtonReleased);
	Button_Quit->OnTabiButtonReleased.RemoveDynamic(this, &ThisClass::OnQuitButtonReleased);

	Super::NativeDestruct();
}

void UTabiMainMenu::OnStartButtonReleased()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
	}

	if (UGameInstance* GI = GetGameInstance())
	{
		if (UTabiGameFlowSubsystem* Subsystem = GI->GetSubsystem<UTabiGameFlowSubsystem>())
		{
			Subsystem->StartNewGame();
		}
	}
}

void UTabiMainMenu::OnQuitButtonReleased()
{
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UTabiGameFlowSubsystem* Subsystem = GI->GetSubsystem<UTabiGameFlowSubsystem>())
		{
			Subsystem->QuitGame();
		}
	}
}
