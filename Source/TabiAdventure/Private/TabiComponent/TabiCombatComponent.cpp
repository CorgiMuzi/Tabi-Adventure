// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiComponent/TabiCombatComponent.h"

#include "TabiAnimation/TabiAnimInstance.h"
#include "TabiCharacter/TabiCharacterBase.h"
#include "TabiData/TabiAttackDefinition.h"
#include "Components/BoxComponent.h"

uint32 UTabiCombatComponent::NextRequestID = 1;

UTabiCombatComponent::UTabiCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTabiCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ATabiCharacterBase* Owner = GetOwner<ATabiCharacterBase>())
	{
		SetupHitbox(Owner);

		AnimInstance = Cast<UTabiAnimInstance>(Owner->GetAnimInstance());
		if (AnimInstance)
		{
			AnimInstance->OnEnableHitCollision.BindDynamic(this, &ThisClass::EnableHitCollision);
			AnimInstance->OnDisableHitCollision.BindDynamic(this, &ThisClass::DisableHitCollision);
			AnimInstance->OnAttackAnimEnd.AddDynamic(this, &ThisClass::FinishAttack);
		}
	}
}

void UTabiCombatComponent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UTabiCombatComponent::SetupHitbox(ATabiCharacterBase* Owner)
{
	Hitbox = Owner->GetHitbox();
	// TODO: Place hitbox in right place based on attack half radius
	FVector HitboxExtent = Hitbox->GetUnscaledBoxExtent();
	HitboxExtent.Y = AttackHalfRadius;
	Hitbox->SetBoxExtent(HitboxExtent);
	Hitbox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnHitboxBeginOverlap);
	Hitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UTabiCombatComponent::EnableHitCollision()
{
	AlreadyHitCharacters.Empty();
	AlreadyHitCharacters.Add(GetOwner<ATabiCharacterBase>());

	if (Hitbox) Hitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UTabiCombatComponent::DisableHitCollision()
{
	if (Hitbox) Hitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UTabiCombatComponent::OnHitboxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATabiCharacterBase* Target = Cast<ATabiCharacterBase>(OtherActor);
	if (!Target) return;

	ATabiCharacterBase* Owner = GetOwner<ATabiCharacterBase>();
	if (!Owner) return;

	// Do nothing if this character is not hostile to Target
	if (Owner->GetTeamAttitudeTowards(*Target) != ETeamAttitude::Hostile) return;

	if (!AlreadyHitCharacters.Contains(Target))
	{
		AlreadyHitCharacters.Add(Target);
		Attack(Target);
	}
}

FTabiRequestID UTabiCombatComponent::TryBeginAttack()
{
	const UTabiAttackDefinition* AttackDef = SelectAttackDefinition();
	if (AttackDef == nullptr) return FTabiRequestID(0);

	if (!AnimInstance.IsValid()) return FTabiRequestID(0);
	const bool IsAnimQueued = AnimInstance->PlayAttackAnimation(AttackDef->GetAnimSequence());
	if (!IsAnimQueued) return FTabiRequestID(0);

	StoreRequestID();
	CurrentAttack = AttackDef;

	return GetCurrentRequestID();
}

const UTabiAttackDefinition* UTabiCombatComponent::SelectAttackDefinition()
{
	// TODO: Make combo attack system. Don't select next attack definition randomly.
	if (AttackDefinitions.IsEmpty()) return nullptr;
	UTabiAttackDefinition* AttackDef = AttackDefinitions[FMath::RandRange(0, AttackDefinitions.Num() - 1)];

	return AttackDef;
}

bool UTabiCombatComponent::Attack(ATabiCharacterBase* Target)
{
	if (!Target) return false;
	return Target->ReceiveDamage(CurrentAttack, GetOwner());
}

void UTabiCombatComponent::FinishAttack(bool IsCompleted)
{
	if (!CurrentRequestID.IsValid()) return;

	const FTabiRequestID EndedRequestID = CurrentRequestID;
	CurrentRequestID = FTabiRequestID();

	OnTabiAttackEnd.Broadcast(EndedRequestID, IsCompleted);
}

void UTabiCombatComponent::StopAttack()
{
	if (!CurrentRequestID.IsValid()) return;

	if (AnimInstance.IsValid())
	{
		AnimInstance->StopAttackAnimation();
	}
	else
	{
		FinishAttack(false);
	}
}

void UTabiCombatComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Hitbox)
	{
		Hitbox->OnComponentBeginOverlap.RemoveDynamic(this, &ThisClass::OnHitboxBeginOverlap);
	}

	Super::EndPlay(EndPlayReason);
}
