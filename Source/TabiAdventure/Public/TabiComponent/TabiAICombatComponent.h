// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TabiCombatComponent.h"
#include "TabiAICombatComponent.generated.h"


UCLASS(ClassGroup=(Tabi), meta=(BlueprintSpawnableComponent))
class TABIADVENTURE_API UTabiAICombatComponent : public UTabiCombatComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTabiAICombatComponent();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual void EnableHitCollision() override;

	inline float GetMinChaseHalfRadius() const { return MinChaseHalfRadius; }
	inline float GetMaxChaseHalfRadius() const { return MaxChaseHalfRadius; }

protected:
	void SpawnProjectile();

	UPROPERTY(EditAnywhere, meta=(ClampMin="0"))
	float MinChaseHalfRadius = 0.f;

	UPROPERTY(EditAnywhere, meta=(ClampMin="0"))
	float MaxChaseHalfRadius = 0.f;
};
