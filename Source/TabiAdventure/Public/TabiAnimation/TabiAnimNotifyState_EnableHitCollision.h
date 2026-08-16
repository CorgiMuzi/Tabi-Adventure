// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDAnimNotifyState.h"
#include "TabiAnimNotifyState_EnableHitCollision.generated.h"

UCLASS()
class TABIADVENTURE_API UTabiAnimNotifyState_EnableHitCollision : public UPaperZDAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void OnNotifyBegin_Implementation(UPaperZDAnimInstance* OwningInstance) const override;
	virtual void OnNotifyEnd_Implementation(UPaperZDAnimInstance* OwningInstance) const override;
};
