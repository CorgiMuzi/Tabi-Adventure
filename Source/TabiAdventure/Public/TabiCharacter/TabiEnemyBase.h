// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TabiCharacterBase.h"
#include "TabiEnemyBase.generated.h"

class UTabiAICombatComponent;
class ATabiEnemyAIController;
class UTabiAttackDefinition;

class UBehaviorTree;
class UAIPerceptionComponent;
class UWidgetComponent;

UCLASS()
class TABIADVENTURE_API ATabiEnemyBase : public ATabiCharacterBase
{
	GENERATED_BODY()

public:
	ATabiEnemyBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual FTabiRequestID RequestAttack(const AActor* Target = nullptr) override;

protected:
	UPROPERTY(Transient)
	TObjectPtr<UTabiAICombatComponent> AICombatComponent;

	UPROPERTY(EditAnywhere, Category="Tabi|AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditAnywhere, Category="Tabi|AI")
	float PatrolHalfRange;

	//~ Vital
	void InitHealthBar();

	UPROPERTY(VisibleAnywhere, Category="Tabi|UI")
	TObjectPtr<UWidgetComponent> HealthBarWidgetComponent;
	//~ End Vital

	virtual void HandleDeathAnimEnd() override;

	//~ Debug
	// Draws every attack definition's range band in PIE 
	UPROPERTY(EditAnywhere, Category="Tabi|Debug")
	bool bDrawAttackRangeDebug = false;

	void DrawAttackRangeDebug() const;
	//~ End Debug

public:
	//~ Getter & Setter
	inline UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
	inline float GetPatrolHalfRange() const { return PatrolHalfRange;}
	float GetAttackRadius() const;
	float GetMinChaseHalfRadius() const;
	float GetMaxChaseHalfRadius() const;
	//~ End Getter & Setter
};
