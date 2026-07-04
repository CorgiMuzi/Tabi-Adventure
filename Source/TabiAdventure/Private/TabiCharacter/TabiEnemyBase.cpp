// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiCharacter/TabiEnemyBase.h"

#include "TabiComponent/TabiAICombatComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

ATabiEnemyBase::ATabiEnemyBase()
{
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	MovementComp->SetPlaneConstraintOrigin(FVector(0.f, 0.f, 0.f));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	AICombatComponent = CreateDefaultSubobject<UTabiAICombatComponent>("AICombatComponent");
}

bool ATabiEnemyBase::HandleAttackInput()
{
	return Super::HandleAttackInput();
}

void ATabiEnemyBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetTabiTeamId(ETabiCharacterTeamID::Enemy);
}

float ATabiEnemyBase::GetAttackRange() const
{
	return AICombatComponent ? AICombatComponent->GetAttackRange() : 0.f;
}

float ATabiEnemyBase::GetMinChaseRadius() const
{
	return AICombatComponent ? AICombatComponent->GetMinChaseRadius() : 0.f;
}

float ATabiEnemyBase::GetMaxChaseRadius() const
{
	return AICombatComponent ? AICombatComponent->GetMaxChaseRadius() : 0.f;
}
