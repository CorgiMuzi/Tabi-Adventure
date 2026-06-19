// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"

#include "TabiCharacterBase.generated.h"

class UTabiCombatComponent;
class UTabiStatComponent;
class UTabiVitalComponent;

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

UCLASS()
class TABIADVENTURE_API ATabiCharacterBase : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	ATabiCharacterBase();

	virtual void BeginPlay() override;

	void ReceiveDamage(float Damage);

protected:
	UPROPERTY()
	TObjectPtr<UTabiVitalComponent> VitalComponent;

	UPROPERTY()
	TObjectPtr<UTabiStatComponent> StatComponent;

	UPROPERTY()
	TObjectPtr<UTabiCombatComponent> CombatComponent;

	//~ Character State
	ETabiCharacterState CharacterState{ETabiCharacterState::Idling};
	//~ End Character State

	//~ Combat
	UPROPERTY(VisibleAnywhere, Category="Tabi|Combat")
	TObjectPtr<UBoxComponent> Hitbox;

	UPROPERTY(EditAnywhere, Category="Tabi|Combat")
	FVector HitboxBaseOffset;

	UPROPERTY(VisibleAnywhere, Category="Tabi|Combat")
	TObjectPtr<UBoxComponent> Hurtbox;
	//~ End Combat


private:
	void OnFacingChanged();

	bool bIsFacingRight = true;

public:
	void SetFacingRight(bool bNewFacingRight);
	FORCEINLINE bool IsFacingRight() const { return bIsFacingRight; }
	FORCEINLINE ETabiCharacterState GetCharacterState() const { return CharacterState; }

	UBoxComponent* GetHitbox() const { return Hitbox; }
};
