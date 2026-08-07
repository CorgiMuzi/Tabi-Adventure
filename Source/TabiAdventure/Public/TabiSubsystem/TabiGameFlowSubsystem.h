// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TabiGameFlowSubsystem.generated.h"

UCLASS()
class TABIADVENTURE_API UTabiGameFlowSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void StartNewGame();
	void GoToCredits();
	void ReturnToMainMenu();
	void RestartCurrentGame();
	void QuitGame();
};
