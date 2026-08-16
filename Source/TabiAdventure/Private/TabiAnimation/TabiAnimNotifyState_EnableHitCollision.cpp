// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiAnimation/TabiAnimNotifyState_EnableHitCollision.h"

#include "TabiAnimation/TabiAnimInstance.h"

void UTabiAnimNotifyState_EnableHitCollision::OnNotifyBegin_Implementation(UPaperZDAnimInstance* OwningInstance) const
{
	Super::OnNotifyBegin_Implementation(OwningInstance);

	if (UTabiAnimInstance* AnimInstance = Cast<UTabiAnimInstance>(OwningInstance))
	{
		AnimInstance->NotifyEnableHitCollision();
	}
}

void UTabiAnimNotifyState_EnableHitCollision::OnNotifyEnd_Implementation(UPaperZDAnimInstance* OwningInstance) const
{
	Super::OnNotifyEnd_Implementation(OwningInstance);

	if (UTabiAnimInstance* AnimInstance = Cast<UTabiAnimInstance>(OwningInstance))
	{
		AnimInstance->NotifyDisableHitCollision();
	}
}
