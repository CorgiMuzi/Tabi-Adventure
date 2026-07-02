// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "PaperZDCharacter.h"
#include "TabiData/TabiCharacterTeamId.h"

#include "TabiCharacterBase.generated.h"

class UAIPerceptionStimuliSourceComponent;
class UAIPerceptionComponent;
class UTabiAnimInstance;
class UTabiCombatComponent;
class UTabiStatComponent;
class UTabiVitalComponent;
class UTabiAttackDefinition;
class UBoxComponent;

UENUM(BlueprintType)
enum class ETabiCharacterState : uint8
{
	Idling UMETA(DisplayName = "Idling"),
	Jumping UMETA(DisplayName = "Jumping"),
	Attacking UMETA(DisplayName = "Attacking"),
	Stunned UMETA(DisplayName = "Stunned"),
	Dead UMETA(DisplayName = "Dead"),

	MAX UMETA(DisplayName = "MAX")
};

DECLARE_DYNAMIC_DELEGATE(FOnTabiVitalSetSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTabiCharacterDeadSignature);

UCLASS()
class TABIADVENTURE_API ATabiCharacterBase : public APaperZDCharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ATabiCharacterBase();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	virtual void Attack();

	void MoveAlongX(float ScaleX);

	void ReceiveDamage(float Damage, const AActor* DamageCauser = nullptr);

	//~ Delegates
	FOnTabiVitalSetSignature OnVitalSet;
	FOnTabiCharacterDeadSignature OnTabiCharacterDead;
	//~ End Delegates

	//~ AI Perception
	virtual void SetTabiTeamId(const ETabiCharacterTeamID& TeamID);
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override { TabiTeamId = TeamID; }
	virtual FGenericTeamId GetGenericTeamId() const override { return TabiTeamId; }
	//~ End AI Perception

protected:
	UFUNCTION()
	virtual void HandleAttackAnimEnd();

	UFUNCTION()
	virtual void HandleDeathAnimEnd();

	UFUNCTION()
	virtual void OnCharacterDead();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTabiVitalComponent> VitalComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTabiStatComponent> StatComponent;

	//~ Character State
	UFUNCTION()
	void HandleSpeedChanged(ETabiStatType StatType, float NewSpeed, float OldSpeed);

	ETabiCharacterState CharacterState{ETabiCharacterState::Idling};
	//~ End Character State

	//~ Combat
	UPROPERTY(EditAnywhere, Category= "Tabi|Combat")
	TArray<TObjectPtr<UTabiAttackDefinition>> AttackDefinitions;

	UPROPERTY(VisibleAnywhere, Category="Tabi|Combat")
	TObjectPtr<UBoxComponent> Hitbox;

	UPROPERTY(VisibleAnywhere, Category="Tabi|Combat")
	FVector HitboxBaseOffset;

	UPROPERTY(VisibleAnywhere, Category="Tabi|Combat")
	TObjectPtr<UBoxComponent> Hurtbox;

	UPROPERTY()
	FTimerHandle HurtEffectTimerHandle;

	UPROPERTY(EditAnywhere, Category="Tabi|Combat")
	float KnockbackStrength;

	UPROPERTY(EditAnywhere, Category="Tabi|Combat")
	float KnockbackLiftSpeed;
	//~ End Combat

	//~ Animation
	UPROPERTY(VisibleAnywhere, Category="Tabi|Animation")
	TObjectPtr<UTabiAnimInstance> TabiAnimInstance;

	UPROPERTY()
	UPaperFlipbookComponent* Flipbook;

	UPROPERTY()
	FLinearColor DefaultColor;
	//~ End Animation

	// AI Perception
	UPROPERTY(VisibleAnywhere, Category= "Tabi|Perception")
	TObjectPtr<UAIPerceptionStimuliSourceComponent> PerceptionStimuliSource;

	FGenericTeamId TabiTeamId;
	//~ AI Perception


private:
	void OnFacingChanged();

	bool bIsFacingRight = true;

public:
	void SetFacingRight(bool bNewFacingRight);
	FORCEINLINE bool IsFacingRight() const { return bIsFacingRight; }
	FORCEINLINE ETabiCharacterState GetCharacterState() const { return CharacterState; }
	bool IsCharacterMovable() const;
	inline bool IsAlive() const;

	UBoxComponent* GetHitbox() const { return Hitbox; }
};
