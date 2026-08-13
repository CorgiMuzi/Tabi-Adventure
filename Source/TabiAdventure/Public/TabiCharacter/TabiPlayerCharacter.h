// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TabiCharacterBase.h"
#include "TabiPlayerCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;

class UTabiAnimInstance;

struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

class UBoxComponent;

UCLASS()
class TABIADVENTURE_API ATabiPlayerCharacter : public ATabiCharacterBase
{
	GENERATED_BODY()

public:
	ATabiPlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	//~ ATabiCharacterBase
	UFUNCTION()
	virtual void HandleAttackInput();

	virtual void HandleDeathAnimEnd() override;
	//~ End ATabiCharacterBase

	//~ Enhanced Input
	void SetGameplayInputEnabled(bool bIsEnabled);
	//~ End Enhanced Input

protected:
	//~ Camera
	UPROPERTY(EditDefaultsOnly, Category= "Tabi|Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditDefaultsOnly, Category= "Tabi|Camera")
	TObjectPtr<UCameraComponent> Camera;
	//~ End Camera

	//~ Player Input
	void Move(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, Category= "Tabi|Input")
	TObjectPtr<UInputMappingContext> LocomotionContext;

	UPROPERTY(EditDefaultsOnly, Category= "Tabi|Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category= "Tabi|Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Tabi|Input")
	TObjectPtr<UInputAction> DodgeAction;

	UPROPERTY(EditDefaultsOnly, Category= "Tabi|Input")
	TObjectPtr<UInputAction> AttackAction;
	//~ End Player Input

	//~ Combat
	virtual void HandleAttackAnimEnd(bool IsCompleted) override;
	//~ End Combat

private:
	//~ Combat
	int32 AttackComboStack = 0;
	//~ End Combat
};
