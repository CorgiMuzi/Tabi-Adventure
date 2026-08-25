// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TabiInteractable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UTabiInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TABIADVENTURE_API ITabiInteractable
{
	GENERATED_BODY()

public:
	virtual void ShowInteractInfo(APawn* Interactor);
	virtual void HideInteractInfo(APawn* Instigator);
	virtual void Interact(APawn* Interactor);	
};
