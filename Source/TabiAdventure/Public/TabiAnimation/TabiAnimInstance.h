// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDAnimInstance.h"
#include "TabiAnimInstance.generated.h"

class ATabiCharacterBase;
class UCharacterMovementComponent;

class UTabiAttackDefinition;

DECLARE_DYNAMIC_DELEGATE(FOnAttackAnimEndSignature);
DECLARE_DYNAMIC_DELEGATE(FEnableHitCollision);
DECLARE_DYNAMIC_DELEGATE(FDisableHitCollision);

UCLASS()
class TABIADVENTURE_API UTabiAnimInstance : public UPaperZDAnimInstance
{
	GENERATED_BODY()

public:
	UTabiAnimInstance();
	virtual void OnInit_Implementation() override;

	virtual void Tick(float DeltaTime) override;

	/// Play attack animation
	/// @param AttackDef
	/// @return Returns whether the animation has correctly been queued.
	bool PlayAttackAnimation(UTabiAttackDefinition* AttackDef);

	UFUNCTION(BlueprintCallable, Category="Tabi|Combat")
	void PlayNotify_EnableHitCollision();
	UFUNCTION(BlueprintCallable, Category="Tabi|Combat")
	void PlayNotify_DisableHitCollision();

	FOnAttackAnimEndSignature OnAttackAnimEnd;
	FEnableHitCollision OnEnableHitCollision;
	FDisableHitCollision OnDisableHitCollision;

protected:
	UPROPERTY()
	TObjectPtr<ATabiCharacterBase> OwningCharacter;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> CharacterMovement;

	UPROPERTY()
	TObjectPtr<UPaperZDAnimationSource> AnimationSource;

	UPROPERTY(BlueprintReadOnly, Category="Tabi|GroundLocomotion")
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category="Tabi|GroundLocomotion")
	bool bIsFalling;

private:
	void HandleAttackEnd(bool bIsCompleted);
};
