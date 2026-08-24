// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "TabiGameFlowSettings.generated.h"

class USoundBase;

UCLASS(config=Game, defaultconfig, meta=(DisplayName="Tabi Game Flow"))
class TABIADVENTURE_API UTabiGameFlowSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

#if WITH_EDITOR
	virtual FName GetCategoryName() const override;
#endif

	UPROPERTY(EditDefaultsOnly, Config, Category="Levels", meta=(AllowedClassed="/Script/Engine.World"))
	TSoftObjectPtr<UWorld> MenuLevel;

	UPROPERTY(EditDefaultsOnly, Config, Category="Levels", meta=(AllowedClassed="/Script/Engine.World"))
	TSoftObjectPtr<UWorld> AdventureLevel;

	UPROPERTY(EditDefaultsOnly, Config, Category="Levels", meta=(AllowedClassed="/Script/Engine.World"))
	TSoftObjectPtr<UWorld> CreditLevel;

	//~ Music
	UPROPERTY(EditDefaultsOnly, Config, Category="Music")
	TSoftObjectPtr<USoundBase> MenuMusic;

	UPROPERTY(EditDefaultsOnly, Config, Category="Music")
	TSoftObjectPtr<USoundBase> AdventureMusic;

	UPROPERTY(EditDefaultsOnly, Config, Category="Music")
	TSoftObjectPtr<USoundBase> CreditMusic;
	
	UPROPERTY(EditDefaultsOnly, Config, Category="Music")
	TSoftObjectPtr<USoundBase> DefeatMusic;

	UPROPERTY(EditDefaultsOnly, Config, Category="Music", meta=(ClampMin="0"))
	float MusicFadeInTime{1.5f};

	UPROPERTY(EditDefaultsOnly, Config, Category="Music", meta=(ClampMin="0"))
	float MusicFadeOutTime{1.f};
	//~ End Music
};
