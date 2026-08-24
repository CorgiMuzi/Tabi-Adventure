// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#include "TabiCharacter/TabiPlayerController.h"

#include "TabiCharacter/TabiPlayerCharacter.h"
#include "TabiWidget/TabiPlayerHUD.h"
#include "TabiGameFramework/TabiAdventureGameModeBase.h"

void ATabiPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	CreatePlayerHUD();

	SetInputMode(FInputModeGameOnly());
	SetShowMouseCursor(false);

	if (ATabiPlayerCharacter* PlayerCharacter = Cast<ATabiPlayerCharacter>(InPawn))
	{
		PlayerCharacter->OnTabiCharacterDead.AddDynamic(this, &ThisClass::HandleCharacterDead);
	}
}

void ATabiPlayerController::OnUnPossess()
{
	if (CurrentHUD)
	{
		CurrentHUD->RemoveFromParent();
		CurrentHUD = nullptr;
	}

	if (ATabiPlayerCharacter* PlayerCharacter = GetPawn<ATabiPlayerCharacter>())
	{
		PlayerCharacter->OnTabiCharacterDead.RemoveDynamic(this, &ThisClass::HandleCharacterDead);
	}

	Super::OnUnPossess();
}

void ATabiPlayerController::HandleCharacterDead()
{
	if (ATabiAdventureGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ATabiAdventureGameModeBase>())
	{
		GameMode->OnPlayerDied(this);
	}
}

void ATabiPlayerController::CreatePlayerHUD()
{
	checkf(PlayerHUDClass != nullptr, TEXT("PlayerController needs PlayerHUD class."));

	ATabiPlayerCharacter* PlayerCharacter = GetPawn<ATabiPlayerCharacter>();
	if (!PlayerCharacter) return;

	CurrentHUD = CreateWidget<UTabiPlayerHUD>(this, PlayerHUDClass);
	if (!CurrentHUD) return;

	CurrentHUD->AddToViewport();
	CurrentHUD->SetOwningVital(PlayerCharacter->GetVitalComponent());
}

void ATabiPlayerController::ShowInteractPrompt(TSubclassOf<UUserWidget> InWidgetClass)
{
	if (!InWidgetClass) return;

	HideInteractPrompt();

	CurrentInteractPrompt = CreateWidget<UUserWidget>(this, InWidgetClass);
	if (CurrentInteractPrompt)
	{
		CurrentInteractPrompt->AddToViewport();
	}
}

void ATabiPlayerController::HideInteractPrompt()
{
	if (!CurrentInteractPrompt) return;

	CurrentInteractPrompt->RemoveFromParent();
	CurrentInteractPrompt = nullptr;
}
