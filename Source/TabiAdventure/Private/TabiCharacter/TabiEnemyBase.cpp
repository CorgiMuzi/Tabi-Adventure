// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiCharacter/TabiEnemyBase.h"

#include "TabiComponent/TabiAICombatComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

ATabiEnemyBase::ATabiEnemyBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UTabiAICombatComponent>(Super::TabiCombatComponentName))
{
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	MovementComp->SetPlaneConstraintOrigin(FVector(0.f, 0.f, 0.f));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ATabiEnemyBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetTabiTeamId(ETabiCharacterTeamID::Enemy);
	AICombatComponent = Cast<UTabiAICombatComponent>(CombatComponent);
}

FTabiRequestID ATabiEnemyBase::RequestAttack()
{
	return Super::RequestAttack();
}

float ATabiEnemyBase::GetAttackRadius() const
{
	return AICombatComponent ? AICombatComponent->GetAttackRadius() : 0.f;
}

float ATabiEnemyBase::GetMinChaseHalfRadius() const
{
	return AICombatComponent ? AICombatComponent->GetMinChaseHalfRadius() : 0.f;
}

float ATabiEnemyBase::GetMaxChaseHalfRadius() const
{
	return AICombatComponent ? AICombatComponent->GetMaxChaseHalfRadius() : 0.f;
}
