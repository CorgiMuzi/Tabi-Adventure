// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TabiEnemyAIController.generated.h"

struct FAIStimulus;
class UAISenseConfig_Sight;

UCLASS()
class TABIADVENTURE_API ATabiEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	ATabiEnemyAIController();
	virtual void PostInitializeComponents() override;

	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void HandleCharacterDeath();

	UFUNCTION()
	void HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	void UpdateTarget(AActor* Target, bool IsSensed);

	//~ AI Perception
	UPROPERTY(EditAnywhere, Category="Tabi|AI|Perception")
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, Category="Tabi|AI|Perception")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UPROPERTY(EditAnywhere, Category="Tabi|AI|Perception")
	float SightRadius = 300.f;

	UPROPERTY(EditAnywhere, Category="Tabi|AI|Perception")
	float LoseSightRadius = 350.f;

	UPROPERTY(EditAnywhere, Category="Tabi|AI|Perception")
	float PeripheralVisionHalfAngle = 180.f;
	//~ End AI Perception
};
