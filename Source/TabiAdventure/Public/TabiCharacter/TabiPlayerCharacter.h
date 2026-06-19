// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TabiCharacterBase.h"
#include "TabiPlayerCharacter.generated.h"

class UTabiAttackDefinition;
class USpringArmComponent;
class UCameraComponent;

class UTabiAnimInstance;

struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

UCLASS()
class TABIADVENTURE_API ATabiPlayerCharacter : public ATabiCharacterBase
{
	GENERATED_BODY()

public:
	ATabiPlayerCharacter();
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	//~ Camera
	UPROPERTY(EditDefaultsOnly, Category= "Tabi|Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditDefaultsOnly, Category= "Tabi|Camera")
	TObjectPtr<UCameraComponent> Camera;
	//~ End Camera

	//~ Anim Instance
	UPROPERTY()
	TObjectPtr<UTabiAnimInstance> PlayerAnimInstance;
	//~ End Anim Instance

	//~ Player Input
	void Move(const FInputActionValue& Value);
	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;
	void Attack();

	UFUNCTION()
	void HandleSpeedChanged(ETabiStatType StatType, float NewSpeed, float OldSpeed);

	UPROPERTY(EditDefaultsOnly, Category= "Tabi|Input")
	TObjectPtr<UInputMappingContext> LocomotionContext;

	UPROPERTY(EditDefaultsOnly, Category= "Tabi|Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category= "Tabi|Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, Category= "Tabi|Input")
	TObjectPtr<UInputAction> AttackAction;
	//~ End Player Input

	//~ Jump Velocity
	float DefaultGravityScale;

	UPROPERTY(EditAnywhere, Category= "Tabi|Input")
	float AscendingGravityScale;

	UPROPERTY(EditAnywhere, Category= "Tabi|Input")
	float ApexVelocityThreshold;

	UPROPERTY(EditAnywhere, Category= "Tabi|Input")
	float ApexGravityScale;

	UPROPERTY(EditAnywhere, Category= "Tabi|Input")
	float FallingGravityScale;
	//~ End Jump Velocity

	//~ Attack
	UPROPERTY(EditAnywhere, Category= "Tabi|Combat")
	TArray<TObjectPtr<UTabiAttackDefinition>> AttackDefinitions;
	//~ End Attack

private:
	UFUNCTION()
	void HandleAttackEnd();

	//~ Attack
	int32 AttackComboStack = 0;
	//~ End Attack

public:
	bool IsCharacterMovable() const;
};
