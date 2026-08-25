// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TabiData/TabiTypes.h"
#include "TabiCombatVFXComponent.generated.h"

class UNiagaraSystem;
class UTabiCombatComponent;

UCLASS(ClassGroup=(Tabi), meta=(BlueprintSpawnableComponent))
class TABIADVENTURE_API UTabiCombatVFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTabiCombatVFXComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void PlayEvadeEffect(const FVector& Location, const FRotator& Rotation) const;

protected:
	void HandleHitConfirmed(const FTabiHitEvent& HitEvent);
	void SpawnOneShotEffect(UNiagaraSystem* Effect, const FVector& Location, const FRotator& Rotation, const FVector& Scale) const;

	UPROPERTY(EditAnywhere, Category="Tabi|VFX")
	TObjectPtr<UNiagaraSystem> DefaultHitEffect;

	UPROPERTY(EditAnywhere, Category="Tabi|VFX")
	TObjectPtr<UNiagaraSystem> EvadeEffect;

	UPROPERTY(EditAnywhere, Category="Tabi|VFX")
	FVector EvadeEffectScale{FVector::OneVector};

private:
	TWeakObjectPtr<UTabiCombatComponent> CombatComponent;
	FDelegateHandle HitConfirmedHandle;
};
