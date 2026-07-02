// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TabiAICombatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TABIADVENTURE_API UTabiAICombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTabiAICombatComponent();

	virtual void BeginPlay() override;

	inline float GetAttackRange() const { return AttackRange; }
	inline float GetMinChaseRadius() const { return MinChaseRadius; }
	inline float GetMaxChaseRadius() const { return MaxChaseRadius; }

protected:
	UPROPERTY(EditAnywhere, meta=(ClampMin="0"))
	float AttackRange;

	UPROPERTY(EditAnywhere, meta=(ClampMin="0"))
	float MinChaseRadius;

	UPROPERTY(EditAnywhere, meta=(ClampMin="0"))
	float MaxChaseRadius;
};
