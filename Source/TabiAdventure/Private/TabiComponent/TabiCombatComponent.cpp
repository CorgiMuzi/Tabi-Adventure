// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiComponent/TabiCombatComponent.h"
#include "TabiAnimation/TabiAnimInstance.h"
#include "TabiCharacter/TabiCharacterBase.h"
#include "Components/BoxComponent.h"

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

		if (UTabiAnimInstance* AnimInstance = Cast<UTabiAnimInstance>(Owner->GetAnimInstance()))
		{
			AnimInstance->OnEnableHitCollision.BindDynamic(this, &ThisClass::EnableHitCollision);
			AnimInstance->OnDisableHitCollision.BindDynamic(this, &ThisClass::DisableHitCollision);
		}
	}
}

void UTabiCombatComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Hitbox->OnComponentBeginOverlap.RemoveDynamic(this, &ThisClass::OnHitboxBeginOverlap);

	Super::EndPlay(EndPlayReason);
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
	if (ATabiCharacterBase* Target = Cast<ATabiCharacterBase>(OtherActor))
	{
		if (AlreadyHitCharacters.Contains(Target)) return;
		AlreadyHitCharacters.Add(Target);
		Attack(Target);
	}
}

void UTabiCombatComponent::SetupHitbox(ATabiCharacterBase* Owner)
{
	Hitbox = Owner->GetHitbox();
	FVector HitboxExtent = Hitbox->GetUnscaledBoxExtent();
	// HitboxExtent.Y = AttackRange / 2.f;
	Hitbox->SetBoxExtent(HitboxExtent);
	Hitbox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnHitboxBeginOverlap);
	Hitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UTabiCombatComponent::Attack(ATabiCharacterBase* Target)
{
	// TODO: Reference Attack Definition later.
	Target->ReceiveDamage(nullptr/*30.f*/, GetOwner());
}
