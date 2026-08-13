// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiCharacter/TabiEnemyBase.h"

#include "TabiComponent/TabiAICombatComponent.h"

#include "TabiWidget/TabiVitalBar.h"

#include "Components/WidgetComponent.h"

ATabiEnemyBase::ATabiEnemyBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UTabiAICombatComponent>(Super::TabiCombatComponentName))
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidgetComponent->SetupAttachment(RootComponent);
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	HealthBarWidgetComponent->SetRelativeLocation(FVector::UpVector * 50.f);
	HealthBarWidgetComponent->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
}

void ATabiEnemyBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetTabiTeamId(ETabiCharacterTeamID::Enemy);
	AICombatComponent = Cast<UTabiAICombatComponent>(CombatComponent);
}

void ATabiEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	InitHealthBar();
}

FTabiRequestID ATabiEnemyBase::RequestAttack()
{
	return Super::RequestAttack();
}

void ATabiEnemyBase::InitHealthBar()
{
	if (!HealthBarWidgetComponent) return;
	UTabiVitalBar* HealthBar = Cast<UTabiVitalBar>(HealthBarWidgetComponent->GetUserWidgetObject());
	if (!HealthBar) return;
	HealthBar->SetOwningVital(GetVitalComponent());
}

void ATabiEnemyBase::HandleDeathAnimEnd()
{
	Super::HandleDeathAnimEnd();

	Destroy();
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
