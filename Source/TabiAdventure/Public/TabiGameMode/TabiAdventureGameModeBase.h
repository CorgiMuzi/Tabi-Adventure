// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TabiAdventureGameModeBase.generated.h"

UENUM(BlueprintType)
enum class ETabiAdventureMatchState : uint8
{
	Playing UMETA(DisplayName="Playing"),
	Won UMETA(DisplayName="Won"),
	Lost UMETA(DisplayName="Lost"),

	MAX UMETA(Hidden)
};

UCLASS()
class TABIADVENTURE_API ATabiAdventureGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ATabiAdventureGameModeBase();

	UFUNCTION()
	void OnPlayerDied();

	void OnPlayerReachedGoal();

	ETabiAdventureMatchState GetMatchState() const { return MatchState; }
protected:
	virtual void BeginPlay() override;

	void HandleVictory();
	void HandleDefeat(APlayerController* InInstigator);

	//~ Credits
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UWorld> CreditWorld;

	FTimerHandle CreditTransitionDelayTimerHandle;

	// How long should it delayed to start fade-out for transitiong to credit world.
	UPROPERTY()
	float CreditTransitionDelay;
	//~ End Credits



	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> DefeatNotifyWidgetClass;
private:

	ETabiAdventureMatchState MatchState;
};
