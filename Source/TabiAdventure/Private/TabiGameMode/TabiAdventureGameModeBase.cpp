// Fill out your copyright notice in the Description page of Project Settings.


#include "TabiGameMode/TabiAdventureGameModeBase.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

ATabiAdventureGameModeBase::ATabiAdventureGameModeBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATabiAdventureGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	MatchState = ETabiAdventureMatchState::Playing;
}

void ATabiAdventureGameModeBase::HandleVictory()
{
	checkf(CreditWorld, TEXT("TabiAdventureGameMode needs ending credit world to travel."));

	GetWorldTimerManager().SetTimer(CreditTransitionDelayTimerHandle, [this]()
	{
		UGameplayStatics::OpenLevel(this, CreditWorld->GetFName(), true);
	}, CreditTransitionDelay, false);
}

void ATabiAdventureGameModeBase::HandleDefeat(APlayerController* InInstigator)
{
	MatchState = ETabiAdventureMatchState::Lost;

	if (!InInstigator || !DefeatNotifyWidgetClass) return;

	UUserWidget* DefeatNotifyWidget = CreateWidget(InInstigator, DefeatNotifyWidgetClass);
	DefeatNotifyWidget->AddToViewport();
	FInputModeUIOnly DefeatInputMode;
	InInstigator->SetInputMode(DefeatInputMode);
}

void ATabiAdventureGameModeBase::OnPlayerDied()
{
	APlayerController* PC = GetInstigatorController<APlayerController>();
	HandleDefeat(PC);
}

void ATabiAdventureGameModeBase::OnPlayerReachedGoal()
{
	if (MatchState != ETabiAdventureMatchState::Playing) return;

	UE_LOG(LogTemp, Warning, TEXT("Player Reached a Goal."));

	MatchState = ETabiAdventureMatchState::Won;
	HandleVictory();
}
