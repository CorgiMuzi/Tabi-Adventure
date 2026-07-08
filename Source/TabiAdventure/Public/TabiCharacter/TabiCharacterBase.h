// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "PaperZDCharacter.h"
#include "TabiData/TabiCharacterTeamId.h"
#include "TabiData/TabiTypes.h"
#include "TabiCharacterBase.generated.h"

class UInputAction;
class UPaperZDAnimSequence;
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

	MAX UMETA(Hidden)
};


DECLARE_DYNAMIC_DELEGATE(FOnTabiVitalSetSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTabiCharacterDeadSignature);

UCLASS()
class TABIADVENTURE_API ATabiCharacterBase : public APaperZDCharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ATabiCharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	bool CanJumpInternal_Implementation() const;
	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;
	void MoveAlongX(float ScaleX);

	//~ Character State
	bool SetCharacterState(ETabiCharacterState NewState);
	FORCEINLINE ETabiCharacterState GetCharacterState() const { return CurrentState; }
	bool CanMove() const;
	inline bool IsAlive() const;
	//~ End Character State

	//~ Combat
	bool CanAttack() const;
	virtual FTabiRequestID RequestAttack();
	void StopAttack();
	/**
	 * Apply damage to this character.
	 * @param AttackDefinition Attack skill contenxt.
	 * @param DamageCauser Who make this attack definition and apply it.
	 * @return	Whether successfully apply damage.
	 */
	bool ReceiveDamage(const UTabiAttackDefinition* AttackDefinition = nullptr, const AActor* DamageCauser = nullptr);

	UBoxComponent* GetHitbox() const { return Hitbox; }
	UTabiCombatComponent* GetCombatComponent() const { return CombatComponent;}
	//~ End Combat

	//~ Delegates
	FOnTabiVitalSetSignature OnVitalSet;
	FOnTabiCharacterDeadSignature OnTabiCharacterDead;
	//~ End Delegates

	//~ AI Perception
	virtual void SetTabiTeamId(const ETabiCharacterTeamID& TeamID);
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override { TabiTeamId = TeamID; }
	virtual FGenericTeamId GetGenericTeamId() const override { return TabiTeamId; }
	const AActor* GetCurrentPlatform() const;
	bool IsOnSamePlatformAs(const AActor* OtherActor) const;
	//~ End AI Perception

	void SetFacingRight(bool bNewFacingRight);
	FORCEINLINE bool IsFacingRight() const { return bIsFacingRight; }

protected:
	UFUNCTION()
	virtual void HandleAttackAnimEnd(bool IsCompleted);

	UFUNCTION()
	virtual void HandleDeathAnimEnd();

	UFUNCTION()
	virtual void OnCharacterDead();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTabiVitalComponent> VitalComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTabiStatComponent> StatComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTabiCombatComponent> CombatComponent;

	static FName TabiCombatComponentName;

	//~ Player Input
	UPROPERTY(EditDefaultsOnly, Category= "Tabi|Input")
	TObjectPtr<UInputAction> JumpAction;
	//~ End Player Input

	//~ Jump
	float DefaultGravityScale;

	UPROPERTY(EditAnywhere, Category= "Tabi|Input")
	float AscendingGravityScale;

	UPROPERTY(EditAnywhere, Category= "Tabi|Input")
	float ApexVelocityThreshold;

	UPROPERTY(EditAnywhere, Category= "Tabi|Input")
	float ApexGravityScale;

	UPROPERTY(EditAnywhere, Category= "Tabi|Input")
	float FallingGravityScale;
	//~ End Jump

	//~ Character State
	void OnCharacterStateChanged(ETabiCharacterState OldState, ETabiCharacterState NewState);

	UFUNCTION()
	void HandleSpeedChanged(ETabiStatType StatType, float NewSpeed, float OldSpeed);

	ETabiCharacterState CurrentState{ETabiCharacterState::Idling};
	//~ End Character State

	//~ Combat
	UPROPERTY(VisibleAnywhere, Category="Tabi")
	TObjectPtr<UBoxComponent> Hitbox;

	UPROPERTY(EditAnywhere, Category="Tabi")
	FVector HitboxBaseOffset;

	UPROPERTY(VisibleAnywhere, Category="Tabi")
	TObjectPtr<UBoxComponent> Hurtbox;

	UPROPERTY()
	FTimerHandle HurtEffectTimerHandle;

	void StartStunTimer(float BaseStunDuration);

	UPROPERTY()
	FTimerHandle StunnedTimerHandle;
	//~ End Combat

	//~ Animation
	UPROPERTY()
	TObjectPtr<UTabiAnimInstance> TabiAnimInstance;

	UPROPERTY()
	UPaperFlipbookComponent* Flipbook;

	UPROPERTY()
	FLinearColor DefaultColor;

	UPROPERTY(EditAnywhere, Category="Tabi|Animation")
	UPaperZDAnimSequence* DeadAnimSequence;
	//~ End Animation

	// AI Perception
	UPROPERTY(VisibleAnywhere, Category= "Tabi|Perception")
	TObjectPtr<UAIPerceptionStimuliSourceComponent> PerceptionStimuliSource;

	FGenericTeamId TabiTeamId;

	UPROPERTY(VisibleAnywhere, Category="Tabi|Perception")
	TObjectPtr<AActor> CurrentPlatform;
	//~ AI Perception

private:
	void OnFacingChanged();

	bool bIsFacingRight = true;
};
