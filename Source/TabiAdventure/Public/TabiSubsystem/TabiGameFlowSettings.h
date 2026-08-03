// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "TabiGameFlowSettings.generated.h"

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
};
