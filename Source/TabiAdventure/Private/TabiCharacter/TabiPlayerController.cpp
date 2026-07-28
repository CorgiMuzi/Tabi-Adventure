// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#include "TabiCharacter/TabiPlayerController.h"
#include "TabiCharacter/TabiPlayerCharacter.h"
#include "TabiWidget/TabiPlayerHUD.h"

void ATabiPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	CreatePlayerHUD();
}

void ATabiPlayerController::OnUnPossess()
{
	if (CurrentHUD)
	{
		CurrentHUD->RemoveFromParent();
		CurrentHUD = nullptr;
	}

	Super::OnUnPossess();
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
