// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "PaperZDCharacter.h"
#include "TabiComponent/TabiVitalComponent.h"
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
	Dodging UMETA(DisplayName = "Dodging"),
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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;
	void MoveAlongX(float ScaleX);

	//~ Character State
	bool SetCharacterState(ETabiCharacterState NewState);
	FORCEINLINE ETabiCharacterState GetCharacterState() const { return CurrentState; }
	bool CanMove() const;
	inline bool IsAlive() const;
	//~ End Character State

	//~ Helper functions to control the character sprite.
	void SetSpriteColor(const FLinearColor& InColor);

	void SetSpriteRelativeLocation(const FVector& InLocation);
	FVector GetSpriteRelativeLocation() const;
	//~ End Sprite Helpers

	//~ Vital
	UTabiVitalComponent* GetVitalComponent() const { return VitalComponent;}
	//~ End Vital

	//~ Combat
	void StopAttack();
	bool CanAttack() const;
	virtual FTabiRequestID RequestAttack();
	/**
	 * Apply damage to this character.
	 * @param AttackDefinition Attack skill contenxt.
	 * @param DamageCauser Who make this attack definition and apply it.
	 * @return	Whether successfully apply damage.
	 */
	bool ReceiveDamage(const UTabiAttackDefinition* AttackDefinition = nullptr, const AActor* DamageCauser = nullptr);

	UBoxComponent* GetHitbox() const { return Hitbox; }
	UBoxComponent* GetHurtbox() const { return Hurtbox; }
	UTabiCombatComponent* GetCombatComponent() const { return CombatComponent; }
	//~ End Combat

	//~ Delegates
	FOnTabiVitalSetSignature OnVitalSet;
	FOnTabiCharacterDeadSignature OnTabiCharacterDead;
	//~ End Delegates

	//~ IGenericTeamAgentInterface
	virtual void SetTabiTeamId(const ETabiCharacterTeamID& TeamID);
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override { TabiTeamId = TeamID; }
	virtual FGenericTeamId GetGenericTeamId() const override { return TabiTeamId; }

	bool IsPlayerTeam() const { return GetGenericTeamId() == FGenericTeamId(static_cast<uint8>(ETabiCharacterTeamID::Player)); }
	//~ End IGenericTeamAgentInterface

	//~ AI Perception
	const AActor* GetCurrentPlatform() const;
	bool IsOnSamePlatformAs(const AActor* OtherActor) const;
	//~ End AI Perception

	/**
	 *
	 * @return Half sizes of the character ( X: width, Y: height )
	 */
	FVector GetCharacterHalfSize() const;
	void FaceToward(const AActor* Target);
	void SetFacingRight(bool bNewFacingRight);
	FORCEINLINE bool IsFacingRight() const { return bIsFacingRight; }

protected:
	UFUNCTION()
	virtual void HandleAttackAnimEnd(bool IsCompleted);

	UFUNCTION()
	virtual void HandleDeathAnimEnd();

	UFUNCTION()
	virtual void OnCharacterDead(const ETabiVitalType& InVitalType);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTabiVitalComponent> VitalComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTabiStatComponent> StatComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTabiCombatComponent> CombatComponent;

	static FName TabiCombatComponentName;

	//~ Jump
	virtual bool CanJumpInternal_Implementation() const override;

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

	//~ Stamina
	/**
	 * Increase the stamina as Amount
	 * !! Never decreased even passed the negative value as the amount. !!
	 * @param DeltaTime How much stamina would be recovered.
	 */
	void TickStaminaRegen(const float DeltaTime);

	/**
	 *
	 * @param Cost How much the stamina used.
	 * @return Whether is successfully consume the stamina
	 */
	bool ConsumeStamina(const float Cost);

	/**
	 * When the stamina regeneration would be start
	 */
	UPROPERTY(EditDefaultsOnly, Category="Tabi|Stamina")
	float StaminaRegenResumeTime;

	/**
	 * How long the stamina regen would be delayed after consuming it.
	 */
	UPROPERTY(EditDefaultsOnly, Category="Tabi|Stamina")
	float StaminaRegenDelay = 0.1f;

	/**
	 * How much the stamina regen per seconds.
	 */
	UPROPERTY(EditDefaultsOnly, Category="Tabi|Stamina")
	float StaminaRegenRate = 1.f;
	//~ End Stamina

	//~ Dodge
	UFUNCTION()
	virtual void Dodge();

	void ExecuteDodge();
	void ExecuteAirDodge();
	void EndAirDodge();
	void EndDodge(float GroundFriction, float BrakingDecelerationWalking);
	bool CanDodge() const;

	UPROPERTY()
	float DodgeDirection;

	UPROPERTY(EditAnywhere, Category="Tabi|Dodge")
	float DodgeSpeed = 500.f;

	/**
	 * The ratio of speed reduction when trying to dodge in air.
	 */
	UPROPERTY(EditAnywhere, Category="Tabi|Dodge", meta=(ClampMin=0, ClampMax=1))
	float AirDodgeSpeedScaler = 0.9f;

	UPROPERTY(EditAnywhere, Category="Tabi|Dodge")
	float DodgeDuration = 1.f;

	UPROPERTY(EditAnywhere, Category="Tabi|Dodge")
	float DodgeRecoveryTime = 0.3f;

	UPROPERTY(EditAnywhere, Category="Tabi|Dodge")
	float DodgeStaminaUsage = 30.f;

	FTimerHandle DodgeExecutionTimerHandle;
	FTimerHandle DodgeInvulnerableTimerHandle;
	FTimerHandle DodgeRecoveryTimerHandle;
	//~ End Dodge

	//~ Character State
	void OnCharacterStateChanged(ETabiCharacterState OldState, ETabiCharacterState NewState);

	UFUNCTION()
	void HandleSpeedChanged(ETabiStatType StatType, float NewSpeed, float OldSpeed);

	ETabiCharacterState CurrentState{ETabiCharacterState::Idling};
	//~ End Character State

	//~ Combat
	void StartStunTimer(float BaseStunDuration, bool ShouldApplyStat = true);

	UPROPERTY(VisibleAnywhere, Category="Tabi")
	TObjectPtr<UBoxComponent> Hitbox;

	UPROPERTY(VisibleAnywhere, Category="Tabi")
	TObjectPtr<UBoxComponent> Hurtbox;

	FTimerHandle HurtEffectTimerHandle;
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

	UPROPERTY(EditDefaultsOnly, Category="Tabi|Animation")
	float DeathFallbackTime;

	bool bDeathHandled{false};

	FTimerHandle DeathFallbackTimerHandle;
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
	bool bNeedRecoverToDodge = false;
	bool bIsAirDodging = false;
};
