// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AnimSequences/PaperZDAnimSequence.h"
#include "Engine/DataAsset.h"
#include "TabiAttackDefinition.generated.h"


UCLASS()
class TABIADVENTURE_API UTabiAttackDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Tabi|Animation")
	TObjectPtr<UPaperZDAnimSequence> AnimSequence;

	UPROPERTY(EditAnywhere, Category="Tabi|Animation")
	float Damage;
};
