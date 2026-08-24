// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TabiInteractor.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UTabiInteractor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TABIADVENTURE_API ITabiInteractor
{
	GENERATED_BODY()

public:
	virtual void ShowInteractInfo(AActor* Instigator);
	virtual void HideInteractInfo(AActor* Instigator);
	virtual void Interact(AActor* Instigator);
};
