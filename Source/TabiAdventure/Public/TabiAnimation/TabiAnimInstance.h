// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDAnimInstance.h"
#include "TabiAnimInstance.generated.h"

class ATabiCharacterBase;
class UCharacterMovementComponent;

class UTabiAttackDefinition;

DECLARE_DYNAMIC_DELEGATE(FOnAttackAnimEndSignature);
DECLARE_DYNAMIC_DELEGATE(FOnDeadAnimEndSignature);
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
	bool PlayDeadAnimation();

	UFUNCTION(BlueprintCallable, Category="Tabi|Combat")
	void PlayNotify_EnableHitCollision();
	UFUNCTION(BlueprintCallable, Category="Tabi|Combat")
	void PlayNotify_DisableHitCollision();

	FOnAttackAnimEndSignature OnAttackAnimEnd;
	FOnDeadAnimEndSignature OnDeathAnimEnd;
	FEnableHitCollision OnEnableHitCollision;
	FDisableHitCollision OnDisableHitCollision;

protected:
	UPROPERTY()
	TObjectPtr<UPaperZDAnimationSource> AnimationSource;

	UPROPERTY(EditDefaultsOnly, Category="Tabi|GroundLocomotion")
	TObjectPtr<UPaperZDAnimSequence> DeadAnimSequence;

	UPROPERTY(BlueprintReadOnly, Category="Tabi|GroundLocomotion")
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category="Tabi|GroundLocomotion")
	bool bIsFalling;

private:
	void HandleAttackAnimEnd(bool bIsCompleted);
	void HandleDeadAnimEnd(bool bIsCompleted);

public:
	inline void SetSpeed(const float NewSpeed) { Speed = NewSpeed; }
	inline void SetIsFalling(const bool IsFalling) { bIsFalling = IsFalling; }
};
