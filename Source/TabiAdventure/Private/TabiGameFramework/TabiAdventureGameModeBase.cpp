// Fill out your copyright notice in the Description page of Project Settings.


#include "TabiGameFramework/TabiAdventureGameModeBase.h"

#include "Blueprint/UserWidget.h"
#include "TabiCharacter/TabiPlayerCharacter.h"
#include "TabiSubsystem/TabiGameFlowSubsystem.h"

ATabiAdventureGameModeBase::ATabiAdventureGameModeBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATabiAdventureGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	MatchState = ETabiAdventureMatchState::Playing;
}

void ATabiAdventureGameModeBase::HandleVictory(APlayerController* InPC)
{
	if (!InPC) return;
	if (InPC->PlayerCameraManager)
	{
		InPC->PlayerCameraManager->StartCameraFade(
			0.f, 1.f, FadeOutTime, FLinearColor::Black,
			true, true
			);
	}

	GetWorldTimerManager().SetTimer(CreditTransitionDelayTimerHandle, [this]()
	{
		if (UTabiGameFlowSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UTabiGameFlowSubsystem>())
		{
			Subsystem->GoToCredits();
		}
	}, CreditTransitionDelay, false);
}

void ATabiAdventureGameModeBase::HandleDefeat(APlayerController* InPC)
{
	MatchState = ETabiAdventureMatchState::Lost;

	if (!InPC || !DefeatNotifyWidgetClass) return;

	UUserWidget* DefeatNotifyWidget = CreateWidget(InPC, DefeatNotifyWidgetClass);
	DefeatNotifyWidget->AddToViewport();
	FInputModeUIOnly DefeatInputMode;
	InPC->SetInputMode(DefeatInputMode);
}

void ATabiAdventureGameModeBase::OnPlayerDied(APlayerController* InPC)
{
	HandleDefeat(InPC);
}

void ATabiAdventureGameModeBase::OnPlayerReachedGoal(APlayerController* InPC)
{
	if (MatchState != ETabiAdventureMatchState::Playing) return;

	UE_LOG(LogTemp, Warning, TEXT("Player Reached a Goal."));

	MatchState = ETabiAdventureMatchState::Won;
	if (ATabiPlayerCharacter* PlayerCharacter = InPC->GetPawn<ATabiPlayerCharacter>())
	{
		PlayerCharacter->SetGameplayInputEnabled(false);
	}
	HandleVictory(InPC);
}
