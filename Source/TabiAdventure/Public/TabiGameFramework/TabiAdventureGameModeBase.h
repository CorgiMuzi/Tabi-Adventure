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

	void OnPlayerDied(APlayerController* InPC);
	void OnPlayerReachedGoal(APlayerController* InPC);

	ETabiAdventureMatchState GetMatchState() const { return MatchState; }

protected:
	virtual void BeginPlay() override;

	void HandleVictory(APlayerController* InPC);
	void HandleDefeat(APlayerController* InPC);

	//~ Ending
	UPROPERTY(EditDefaultsOnly, Category="Tabi|GameMode")
	float FadeOutTime;
	//~ End Ending

	//~ Credits
	FTimerHandle CreditTransitionDelayTimerHandle;

	// How long should it delayed to start fade-out for transitiong to credit world.
	UPROPERTY(EditDefaultsOnly, Category="Tabi|GameMode")
	float CreditTransitionDelay;
	//~ End Credits

	UPROPERTY(EditDefaultsOnly, Category="Tabi|GameMode")
	TSubclassOf<class UUserWidget> DefeatNotifyWidgetClass;

private:
	UPROPERTY(VisibleAnywhere, Category="Tabi|GameMode")
	ETabiAdventureMatchState MatchState;
};
