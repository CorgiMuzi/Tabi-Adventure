// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TabiDepthSortSubsystem.generated.h"

class ATabiCharacterBase;

UCLASS()
class TABIADVENTURE_API UTabiDepthSortSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void BeginDestroy() override;

	//~ UTickableWorldSubsystem
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	//~ End UTickableWorldSubsystem

	void RegisterActor(ATabiCharacterBase* Actor);
	void UnregisterActor(ATabiCharacterBase* Actor);

private:
	UPROPERTY()
	TArray<TWeakObjectPtr<ATabiCharacterBase>> RegisteredActors;

	float DepthYStep = 0.1f;
	float DepthYBase = 0.f;
};
