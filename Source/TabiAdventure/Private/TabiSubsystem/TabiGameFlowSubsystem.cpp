// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiSubsystem/TabiGameFlowSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "TabiSubsystem/TabiGameFlowSettings.h"

void UTabiGameFlowSubsystem::StartNewGame()
{
	const UTabiGameFlowSettings* Settings = GetDefault<UTabiGameFlowSettings>();
	if (!ensure(Settings && !Settings->AdventureLevel.IsNull())) return;

	UGameplayStatics::OpenLevelBySoftObjectPtr(this, Settings->AdventureLevel, true);
}

void UTabiGameFlowSubsystem::GoToCredits()
{
	const UTabiGameFlowSettings* Settings = GetDefault<UTabiGameFlowSettings>();
	if (!ensure(Settings && !Settings->AdventureLevel.IsNull())) return;

	UGameplayStatics::OpenLevelBySoftObjectPtr(this, Settings->CreditLevel, true);
}

void UTabiGameFlowSubsystem::ReturnToMainMenu()
{
	const UTabiGameFlowSettings* Settings = GetDefault<UTabiGameFlowSettings>();
	if (!ensure(Settings && !Settings->AdventureLevel.IsNull())) return;
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, Settings->MenuLevel, true);
}

void UTabiGameFlowSubsystem::RestartCurrentGame()
{
	const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
	UGameplayStatics::OpenLevel(this, FName(CurrentLevelName), true);
}
