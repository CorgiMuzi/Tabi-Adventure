// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TabiCombatComponent.generated.h"

UCLASS(ClassGroup=(Tabi), meta=(BlueprintSpawnableComponent))
class TABIADVENTURE_API UTabiCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTabiCombatComponent();
	virtual void BeginPlay() override;
};
