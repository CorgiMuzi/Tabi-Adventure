// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TabiCharacterBase.h"
#include "TabiEnemyBase.generated.h"

class UTabiAICombatComponent;
class ATabiEnemyAIController;
class UBehaviorTree;
class UTabiAttackDefinition;
class UAIPerceptionComponent;

UCLASS()
class TABIADVENTURE_API ATabiEnemyBase : public ATabiCharacterBase
{
	GENERATED_BODY()

public:
	ATabiEnemyBase();
	virtual void PostInitializeComponents() override;

	virtual void Attack() override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTabiAICombatComponent> AICombatComponent;

	UPROPERTY(EditAnywhere, Category="Tabi|AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditAnywhere, Category="Tabi|AI")
	float PatrolHalfRange;

private:

public:
	//~ Getter & Setter
	inline UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
	inline float GetPatrolHalfRange() const { return PatrolHalfRange;}
	float GetAttackRange() const;
	float GetMinChaseRadius() const;
	float GetMaxChaseRadius() const;
	//~ End Getter & Setter
};
