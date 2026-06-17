// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "TabiPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class TABIADVENTURE_API ATabiPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* InPawn) override;

	//~ Player Input

	//~ End Player Input
};
