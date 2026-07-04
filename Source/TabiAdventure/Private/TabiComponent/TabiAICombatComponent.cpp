// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiComponent/TabiAICombatComponent.h"
#include "TabiCharacter/TabiEnemyBase.h"
#include "TabiAnimation/TabiAnimInstance.h"
#include "Components/BoxComponent.h"

UTabiAICombatComponent::UTabiAICombatComponent()
{

}

void UTabiAICombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ATabiEnemyBase* Owner = GetOwner<ATabiEnemyBase>())
	{
		Hitbox = Owner->GetHitbox();
		FVector HitboxExtent = Hitbox->GetUnscaledBoxExtent();
		HitboxExtent.Y = AttackRange / 2.f;
		Hitbox->SetBoxExtent(HitboxExtent);

		Hitbox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnHitboxBeginOverlap);
		Hitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (UTabiAnimInstance* AnimInstance = Cast<UTabiAnimInstance>(Owner->GetAnimInstance()))
		{
			AnimInstance->OnEnableHitCollision.BindDynamic(this, &ThisClass::EnableHitCollision);
			AnimInstance->OnDisableHitCollision.BindDynamic(this, &ThisClass::DisableHitCollision);
		}
	}
}

void UTabiAICombatComponent::Attack(ATabiCharacterBase* Target)
{
	if (!Target) return;
	Target->ReceiveDamage(nullptr/*30.f*/,GetOwner());
}

void UTabiAICombatComponent::EnableHitCollision()
{
	AlreadyHitCharacters.Empty();
	AlreadyHitCharacters.Add(GetOwner<ATabiCharacterBase>());

	if (Hitbox) Hitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UTabiAICombatComponent::DisableHitCollision()
{
	if (Hitbox) Hitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UTabiAICombatComponent::OnHitboxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATabiCharacterBase* OtherCharacter = Cast<ATabiCharacterBase>(OtherActor);
	if (!OtherCharacter || OtherCharacter->GetTeamAttitudeTowards(*GetOwner()) != ETeamAttitude::Hostile) return;

	if (AlreadyHitCharacters.Contains(OtherCharacter)) return;
	AlreadyHitCharacters.Add(OtherCharacter);
	Attack(OtherCharacter);
}

