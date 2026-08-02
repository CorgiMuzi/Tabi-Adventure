// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TabiGoalTrigger.generated.h"

class UBoxComponent;

UCLASS()
class TABIADVENTURE_API ATabiGoalTrigger : public AActor
{
	GENERATED_BODY()

public:
	ATabiGoalTrigger();

protected:
	UFUNCTION()
	void OnGoalBoundaryBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, Category="Tabi|GoalTrigger")
	TObjectPtr<UBoxComponent> GoalBoundary;
};


