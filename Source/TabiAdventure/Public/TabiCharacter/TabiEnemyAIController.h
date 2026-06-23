// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TabiEnemyAIController.generated.h"

UCLASS()
class TABIADVENTURE_API ATabiEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	ATabiEnemyAIController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void HandleCharacterDeath();
};
