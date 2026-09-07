// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TabiData/TabiTypes.h"
#include "TabiCombatComponent.generated.h"

class UTabiAnimInstance;
class UPaperFlipbookComponent;
class UTabiAttackDefinition;
class ATabiCharacterBase;
class UBoxComponent;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTabiAttackEndSignature, const FTabiRequestID /*RequestID*/, bool /*bSuccess*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTabiHitConfirmedSignature, const FTabiHitEvent& /*HitEvent*/);

UCLASS(ClassGroup=(Tabi), meta=(BlueprintSpawnableComponent))
class TABIADVENTURE_API UTabiCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTabiCombatComponent();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual FTabiRequestID TryBeginAttack(const FTabiAttackContext& Context);
	virtual ETabiHitResult Attack(ATabiCharacterBase* Target/*, const UTabiAttackDefinition* AttackDefinition*/);

	//~ Attack availability
	// True when at least one definition can be used in this situation.
	bool HasUsableAttack(const FTabiAttackContext& Context) const;

	// Check cooldown.
	bool IsAttackReady(const UTabiAttackDefinition* AttackDefinition) const;

	/**
	 * @return False when no definition applies at all.
	 */
	bool GetAttackDistanceBand(const FTabiAttackContext& Context, float& OutMinRange, float& OutMaxRange) const;

	inline const TArray<TObjectPtr<UTabiAttackDefinition>>& GetAttackDefinitions() const { return AttackDefinitions; }
	//~ End Attack availability

	UFUNCTION()
	void FinishAttack(bool IsCompleted);
	virtual void StopAttack();

	inline const UTabiAttackDefinition* GetCurrentAttack() const { return CurrentAttack; }

	UFUNCTION()
	virtual void EnableHitCollision();

	UFUNCTION()
	virtual void DisableHitCollision();

	bool ApplyPoiseDamage(const float PoiseDamage);

	void OnCharacterDamaged(const UTabiAttackDefinition* AttackDefinition, AActor* DamagedActor, const AActor* DamageCauser);

	void StartHitFlash();
	
	void StartHitBlink(const float Duration);

	void SetVulnerability(const bool InVulnerability) { bIsVulnerable = InVulnerability; }
	bool IsVulnerable() const { return bIsVulnerable; }

	inline float GetAttackRadius() const { return AttackRadius; }
	inline FVector GetHitboxBaseOffset() const { return HitboxBaseOffset; }

	inline static uint32 GetNextRequestID() { return NextRequestID++; }
	inline FTabiRequestID GetCurrentRequestID() const { return CurrentRequestID; }
	inline void StoreRequestID() { CurrentRequestID = GetNextRequestID(); }

	FOnTabiAttackEndSignature OnTabiAttackEnd;
	FOnTabiHitConfirmedSignature OnTabiHitConfirmed;

protected:
	//~ Animation
	UPROPERTY()
	TSoftObjectPtr<UTabiAnimInstance> AnimInstance;
	//~ End Animation

	//~ Stunning
	UPROPERTY(EditDefaultsOnly)
	float HitFlashInterval{0.07f};

	UPROPERTY(EditDefaultsOnly, meta=(ClampMin="0"))
	float HitFlashDuration{0.15f};

	UPROPERTY(EditDefaultsOnly, meta=(ClampMin="0"))
	float PoiseResetDelay{2.f};

	float AccumulatedPoiseDamage{0.f};
	float LastPoiseDamageTime{0.f};
	//~ End Stunning

	//~ Hitbox
	UFUNCTION()
	virtual void OnHitboxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetupHitbox(ATabiCharacterBase* Owner);

	FVector ComputeHitLocation(const AActor* Target, const UPrimitiveComponent* TargetComp) const;
	FRotator ComputeHitRotation(const AActor* Target) const;

	UPROPERTY()
	TObjectPtr<UBoxComponent> Hitbox;

	UPROPERTY()
	FVector HitboxBaseOffset;

	UPROPERTY()
	bool bIsVulnerable{true};
	//~ End Hitbox

	UPROPERTY()
	TArray<TObjectPtr<AActor>> AlreadyHitCharacters;

	/**
	 * How far the melee hitbox reaches past the character's own half width.
	 * This only sizes the hitbox. The distance the AI uses to decide whether it may
	 * attack lives on UTabiAttackDefinition (MinAttackRange / MaxAttackRange).
	 */
	UPROPERTY(EditAnywhere, meta=(ClampMin="0"))
	float AttackRadius = 20.f;

	/**
	 * Select which attack definition should character use when it handles attack.
	 * Picks at random among the definitions that are usable in this situation, so the
	 * choice can never land on an attack the AI was not allowed to start.
	 * @return Selected attack definition
	 */
	virtual const UTabiAttackDefinition* SelectAttackDefinition(const FTabiAttackContext& Context);

	// Fills OutAttacks with every definition usable in this situation.
	void GatherUsableAttacks(const FTabiAttackContext& Context, TArray<const UTabiAttackDefinition*>& OutAttacks) const;

	UPROPERTY(EditAnywhere, Category= "Tabi|Combat")
	TArray<TObjectPtr<UTabiAttackDefinition>> AttackDefinitions;

	UPROPERTY()
	TObjectPtr<const UTabiAttackDefinition> CurrentAttack;

	// Who the current attack was started against.
	TWeakObjectPtr<const AActor> CurrentTarget;

	// World time each definition becomes usable again.
	TMap<TObjectPtr<const UTabiAttackDefinition>, float> AttackReadyTime;

private:
	static uint32 NextRequestID;
	FTabiRequestID CurrentRequestID;
	FTabiRequestID FirstHitRequestID;

	//~ Stunning
	FTimerHandle HitFlashIntervalTimerHandle;
	FTimerHandle HitFlashDurationTimerHandle;

	bool bFlashFlag = true;
	//~ End Stunning
};
