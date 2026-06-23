// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TabiCharacterBase.h"
#include "TabiEnemyBase.generated.h"

class ATabiEnemyAIController;
class UBehaviorTree;

UCLASS()
class TABIADVENTURE_API ATabiEnemyBase : public ATabiCharacterBase
{
	GENERATED_BODY()

public:
	ATabiEnemyBase();

protected:
	UPROPERTY(EditAnywhere, Category="Tabi|AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditAnywhere, Category="Tabi|AI")
	float PatrolHalfRange;


private:

public:
	//~ Getter & Setter
	inline UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
	float GetPatrolHalfRange() const { return PatrolHalfRange;}
	//~ End Getter & Setter
};
