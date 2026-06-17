// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TabiCharacterBase.h"
#include "TabiPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

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

	//~ Player Input
	void Move(const FInputActionValue& Value);
	virtual void Jump() override;

	UFUNCTION()
	void HandleSpeedChanged(ETabiStatType StatType, float NewSpeed, float OldSpeed);

	UPROPERTY(EditDefaultsOnly, Category= "Tabi|Input")
	TObjectPtr<UInputMappingContext> LocomotionContext;

	UPROPERTY(EditDefaultsOnly, Category= "Tabi|Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category= "Tabi|Input")
	TObjectPtr<UInputAction> JumpAction;
	//~ End Player Input
};
