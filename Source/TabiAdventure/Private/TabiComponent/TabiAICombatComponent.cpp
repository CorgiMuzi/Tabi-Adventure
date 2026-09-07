// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiComponent/TabiAICombatComponent.h"

#include "TabiActor/TabiProjectile.h"
#include "TabiCharacter/TabiCharacterBase.h"
#include "TabiData/TabiAttackDefinition.h"

UTabiAICombatComponent::UTabiAICombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

#if WITH_EDITOR
void UTabiAICombatComponent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

void UTabiAICombatComponent::EnableHitCollision()
{
	if (CurrentAttack && CurrentAttack->IsRanged())
	{
		SpawnProjectile();
		return;
	}

	Super::EnableHitCollision();
}

void UTabiAICombatComponent::SpawnProjectile()
{
	UWorld* World = GetWorld();
	ATabiCharacterBase* Owner = GetOwner<ATabiCharacterBase>();
	if (!World || !Owner || !CurrentAttack) return;

	const TSubclassOf<ATabiProjectile> ProjectileClass = CurrentAttack->GetProjectileClass();
	if (!ProjectileClass) return;
	
	const float FacingSign = Owner->IsFacingRight() ? 1.f : -1.f;
	FVector MuzzleOffset = CurrentAttack->GetMuzzleOffset();
	MuzzleOffset.X *= FacingSign;

	const FVector SpawnLocation = Owner->GetActorLocation() + MuzzleOffset;

	FVector Direction = FVector(FacingSign, 0.f, 0.f);
	if (const AActor* Target = CurrentTarget.Get())
	{
		FVector ToTarget = Target->GetActorLocation() - SpawnLocation;
		ToTarget.Y = 0.f;

		if (!ToTarget.IsNearlyZero()) Direction = ToTarget.GetSafeNormal();
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Owner;
	SpawnParams.Instigator = Owner;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ATabiProjectile* Projectile = World->SpawnActor<ATabiProjectile>(ProjectileClass, SpawnLocation, Direction.Rotation(), SpawnParams);
	if (!Projectile) return;

	Projectile->Fire(Owner, Direction, CurrentAttack);
}
