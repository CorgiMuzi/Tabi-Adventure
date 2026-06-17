// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDAnimInstance.h"
#include "TabiAnimInstance.generated.h"

class ATabiCharacterBase;
class UCharacterMovementComponent;

UCLASS()
class TABIADVENTURE_API UTabiAnimInstance : public UPaperZDAnimInstance
{
	GENERATED_BODY()

public:
	UTabiAnimInstance();
	virtual void OnInit_Implementation() override;

	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY()
	TObjectPtr<ATabiCharacterBase> OwningCharacter;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> CharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category="Tabi|GroundLocomotion")
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category="Tabi|GroundLocomotion")
	bool bIsFalling;
};
