// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TabiPlayerController.generated.h"

class UTabiPlayerHUD;

UCLASS()
class TABIADVENTURE_API ATabiPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	void CreatePlayerHUD();

	UPROPERTY(EditDefaultsOnly, Category="Tabi|UI")
	TSubclassOf<UTabiPlayerHUD> PlayerHUDClass;

	UPROPERTY()
	TObjectPtr<UTabiPlayerHUD> CurrentHUD;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	UFUNCTION()
	void HandleCharacterDead();
};
