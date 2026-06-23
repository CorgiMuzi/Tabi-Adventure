// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiCharacter/TabiEnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"

ATabiEnemyBase::ATabiEnemyBase()
{
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	MovementComp->SetPlaneConstraintOrigin(FVector(0.f, 0.f, 0.f));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
