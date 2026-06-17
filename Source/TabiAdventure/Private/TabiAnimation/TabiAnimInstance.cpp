// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiAnimation/TabiAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "TabiCharacter/TabiCharacterBase.h"


UTabiAnimInstance::UTabiAnimInstance()
{
}

void UTabiAnimInstance::OnInit_Implementation()
{
	Super::OnInit_Implementation();

	OwningCharacter = Cast<ATabiCharacterBase>(GetOwningActor());
	if (OwningCharacter)
	{
		CharacterMovement = OwningCharacter->GetCharacterMovement();
	}
}

void UTabiAnimInstance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OwningCharacter)
	{
		Speed = FMath::Abs(OwningCharacter->GetVelocity().Size2D());
	}

	if (CharacterMovement)
	{
		bIsFalling = CharacterMovement->IsFalling();
	}
}
