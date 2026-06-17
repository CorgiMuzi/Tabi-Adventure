// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"

#include "TabiCharacterBase.generated.h"

class UTabiCombatComponent;
class UTabiStatComponent;
class UTabiVitalComponent;

UCLASS()
class TABIADVENTURE_API ATabiCharacterBase : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	ATabiCharacterBase();

	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	TObjectPtr<UTabiVitalComponent> VitalComponent;

	UPROPERTY()
	TObjectPtr<UTabiStatComponent> StatComponent;

	UPROPERTY()
	TObjectPtr<UTabiCombatComponent> CombatComponent;

private:
	void OnFacingChanged();

	bool bIsFacingRight = true;

public:
	void SetFacingRight(bool bNewFacingRight);
	FORCEINLINE bool IsFacingRight() const { return bIsFacingRight; }
};
