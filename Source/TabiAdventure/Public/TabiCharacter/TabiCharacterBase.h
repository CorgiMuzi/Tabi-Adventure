// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"

#include "TabiCharacterBase.generated.h"

class UTabiAnimInstance;
class UTabiCombatComponent;
class UTabiStatComponent;
class UTabiVitalComponent;

class UBoxComponent;

UENUM(BlueprintType)
enum class ETabiCharacterState : uint8
{
	Idling UMETA(DisplayName = "Idling"),
	Jumping UMETA(DisplaWyName = "Jumping"),
	Attacking UMETA(DisplayName = "Attacking"),
	Stunned UMETA(DisplayName = "Stunned"),
	Dead UMETA(DisplayName = "Dead"),

	MAX UMETA(DisplayName = "MAX")
};

UCLASS()
class TABIADVENTURE_API ATabiCharacterBase : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	ATabiCharacterBase();

	virtual void BeginPlay() override;

	void ReceiveDamage(float Damage, const AActor* DamageCauser = nullptr);

protected:
	UFUNCTION()
	virtual void HandleAttackAnimEnd();

	UFUNCTION()
	virtual void HandleDeathAnimEnd();

	UFUNCTION()
	void OnCharacterDead();

	UPROPERTY(VisibleAnywhere, Category="Tabi|Vital")
	TObjectPtr<UTabiVitalComponent> VitalComponent;

	UPROPERTY(VisibleAnywhere, Category="Tabi|Stat")
	TObjectPtr<UTabiStatComponent> StatComponent;

	UPROPERTY(VisibleAnywhere, Category="Tabi|Combat")
	TObjectPtr<UTabiCombatComponent> CombatComponent;

	//~ Character State
	ETabiCharacterState CharacterState{ETabiCharacterState::Idling};
	//~ End Character State

	//~ Combat
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
private:
	void OnFacingChanged();

	bool bIsFacingRight = true;

public:
	void SetFacingRight(bool bNewFacingRight);
	FORCEINLINE bool IsFacingRight() const { return bIsFacingRight; }
	FORCEINLINE ETabiCharacterState GetCharacterState() const { return CharacterState; }

	UBoxComponent* GetHitbox() const { return Hitbox; }
};

