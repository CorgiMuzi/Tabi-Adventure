// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AnimSequences/PaperZDAnimSequence.h"
#include "Engine/DataAsset.h"
#include "TabiAttackDefinition.generated.h"

class UNiagaraSystem;


UCLASS()
class TABIADVENTURE_API UTabiAttackDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	inline const UPaperZDAnimSequence* GetAnimSequence() const { return AnimSequence; }
	inline float GetDamage() const { return Damage; }
	inline float GetKnockbackStrength() const { return KnockbackStrength; }
	inline float GetKnockbackLiftSpeed() const { return KnockbackLiftSpeed; }
	inline float GetHitStunDuration() const { return HitStunDuration; }
	inline UNiagaraSystem* GetHitEffect() const { return HitEffect; }
	inline FVector GetHitEffectScale() const { return HitEffectScale; }

protected:
	UPROPERTY(EditAnywhere, Category="Tabi|Animation")
	TObjectPtr<UPaperZDAnimSequence> AnimSequence;

	UPROPERTY(EditAnywhere, Category="Tabi|Attack")
	float Damage;

	// The horizontal force/speed applied to push the target away.
	UPROPERTY(EditAnywhere, Category="Tabi|Attack")
	float KnockbackStrength;

	// The initial upward velocity applied to the target.
	UPROPERTY(EditAnywhere, Category="Tabi|Attack")
	float KnockbackLiftSpeed;

	// The base value how long the target should be stunned.
	UPROPERTY(EditAnywhere, Category="Tabi|Attack", meta=(ClampMin="0.1"))
	float HitStunDuration;

	UPROPERTY(EditAnywhere, Category="Tabi|Effect")
	TObjectPtr<UNiagaraSystem> HitEffect;

	UPROPERTY(EditAnywhere, Category="Tabi|Effect")
	FVector HitEffectScale{FVector::OneVector};
};
