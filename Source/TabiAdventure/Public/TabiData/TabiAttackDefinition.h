// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AnimSequences/PaperZDAnimSequence.h"
#include "Engine/DataAsset.h"
#include "TabiData/TabiTypes.h"
#include "TabiAttackDefinition.generated.h"

class ATabiProjectile;
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
	inline float GetPoiseDamage() const { return PoiseDamage; }
	inline UNiagaraSystem* GetHitEffect() const { return HitEffect; }
	inline FVector GetHitEffectScale() const { return HitEffectScale; }
	
	//~ Projectile
	inline TSubclassOf<ATabiProjectile> GetProjectileClass() const { return ProjectileClass; } 
	inline float GetProjectileSpeed() const  { return ProjectileSpeed; }
	inline float GetProjectileLifeTime() const { return ProjectileLifeTime; }
	inline USoundBase* GetProjectileHitSound() const { return ProjectileHitSound; }
	inline FVector GetMuzzleOffset() const { return MuzzleOffset; }

	bool IsRanged() const;
	//~ End Projectile

	//~ Range
	inline float GetMinAttackRange() const { return MinAttackRange; }
	inline float GetMaxAttackRange() const { return MaxAttackRange; }
	inline bool RequiresSamePlatform() const { return bRequireSamePlatform; }
	inline float GetCooldown() const { return Cooldown; }

	bool IsUsableAt(const FTabiAttackContext& Context) const;
	//~ End Range

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
	
	UPROPERTY(EditAnywhere, Category="Tabi|Attack", meta=(ClampMin="0"))
	float PoiseDamage = 1.f;

	UPROPERTY(EditAnywhere, Category="Tabi|Effect")
	TObjectPtr<UNiagaraSystem> HitEffect;

	UPROPERTY(EditAnywhere, Category="Tabi|Effect")
	FVector HitEffectScale{FVector::OneVector};

	//~ Projectile 
	UPROPERTY(EditAnywhere, Category="Tabi|Projectile")
	TSubclassOf<ATabiProjectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere, Category="Tabi|Projectile")
	float ProjectileSpeed = 0.f;
	
	UPROPERTY(EditAnywhere, Category="Tabi|Projectile")
	float ProjectileLifeTime = 0.f;
	
	UPROPERTY(EditAnywhere, Category="Tabi|Projectile")
	TObjectPtr<USoundBase> ProjectileHitSound;
	
	UPROPERTY(EditAnywhere, Category="Tabi|Projectile")
	FVector MuzzleOffset{FVector::ZeroVector};

	//~ Range
	// Distances are measured horizontally (X only), from actor origin to actor origin.
	UPROPERTY(EditAnywhere, Category="Tabi|Range", meta=(ClampMin="0"))
	float MinAttackRange = 0.f;
	
	UPROPERTY(EditAnywhere, Category="Tabi|Range", meta=(ClampMin="0"))
	float MaxAttackRange = 100.f;
	
	UPROPERTY(EditAnywhere, Category="Tabi|Range")
	bool bRequireSamePlatform = true;
	
	UPROPERTY(EditAnywhere, Category="Tabi|Attack", meta=(ClampMin="0"))
	float Cooldown = 0.f;
};
