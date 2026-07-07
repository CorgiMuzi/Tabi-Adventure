// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDAnimInstance.h"
#include "TabiAnimInstance.generated.h"

class ATabiCharacterBase;
class UCharacterMovementComponent;

class UTabiAttackDefinition;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttackAnimEndSignature, bool /*IsCompleted*/);
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
	/// @param AttackAnimSequence
	/// @return Returns whether the animation has correctly been queued.
	bool PlayAttackAnimation(const UPaperZDAnimSequence* AttackAnimSequence);
	bool PlayDeadAnimation(const UPaperZDAnimSequence* DeadAnimSequence);

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

	UPROPERTY(BlueprintReadOnly, Category="Tabi|GroundLocomotion")
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category="Tabi|GroundLocomotion")
	bool bIsFalling;

private:
	void HandleAttackAnimEnd(bool IsCompleted);
	void HandleDeadAnimEnd(bool IsCompleted);

public:
	inline void SetSpeed(const float NewSpeed) { Speed = NewSpeed; }
	inline void SetIsFalling(const bool IsFalling) { bIsFalling = IsFalling; }
};
