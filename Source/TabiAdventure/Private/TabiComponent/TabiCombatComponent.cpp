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
	if (!Hitbox) return;

	FVector HitboxExtent{Owner->GetCharacterHalfSize()};
	HitboxExtent.X += AttackRadius / 2.f;
	Hitbox->SetBoxExtent(HitboxExtent);

	HitboxBaseOffset = FVector(AttackRadius / 2.f, 0.f, 0.f);
	Owner->ApplyHitboxOffset(HitboxBaseOffset);

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

void UTabiCombatComponent::OnCharacterDamaged(const UTabiAttackDefinition* AttackDefinition, AActor* DamagedActor, const AActor* DamageCauser)
{
	StopAttack();

	if (!DamagedActor) return;

	if (ATabiCharacterBase* DamagedCharacter = Cast<ATabiCharacterBase>(DamagedActor))
	{
		DamagedCharacter->SetCharacterState(ETabiCharacterState::Stunned);
	}

	if (AttackDefinition)
	{
		StartStunTimer(DamagedActor, AttackDefinition->GetHitStunDuration());
	}
}

void UTabiCombatComponent::StartStunTimer(AActor* StunnedActor, const float BaseStunDuration, const bool ShouldApplyStat)
{
	const float FinalStunDuration = BaseStunDuration;
	if (ShouldApplyStat)
	{
		/**
		 * Calculate final stun duration when stat should be applied.
		 */
	}

	if (FinalStunDuration <= 0.f) return;

	if (UWorld* World = GetWorld())
	{
		TWeakObjectPtr<UTabiCombatComponent> WeakThis(this);

		// Start shaking and blinking effect on the stunned character
		World->GetTimerManager().SetTimer(StunActivationTimerHandle, [WeakThis, StunnedActor]()
		{
			if (!WeakThis.IsValid()) return;
			const UWorld* InWorld = WeakThis->GetWorld();
			if (!InWorld) return;
			InWorld->GetTimerManager().ClearTimer(WeakThis->StunEffectBlinkTimerHandle);

			if (ATabiCharacterBase* StunnedCharacter = Cast<ATabiCharacterBase>(StunnedActor))
			{
				// Reset sprite color
				StunnedCharacter->SetSpriteColor(FLinearColor::White);
				WeakThis->bBlinkFlag = true;

				// Finish stunned state
				StunnedCharacter->SetCharacterState(ETabiCharacterState::Idling);
			}
		}, FinalStunDuration, false);

		// Blink character
		World->GetTimerManager().SetTimer(StunEffectBlinkTimerHandle, [WeakThis, StunnedActor]()
		{
			if (!WeakThis.IsValid() || StunnedActor == nullptr) return;
			ATabiCharacterBase* StunnedCharacter = Cast<ATabiCharacterBase>(StunnedActor);
			if (!StunnedCharacter) return;
			FLinearColor SpriteColor = WeakThis->bBlinkFlag ? FLinearColor::Red : FLinearColor::White;
			StunnedCharacter->SetSpriteColor(SpriteColor);
			WeakThis->bBlinkFlag = !WeakThis->bBlinkFlag;
		}, StunEffectBlinkPeriod, true);
	}
}

void UTabiCombatComponent::OnHitboxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATabiCharacterBase* Target = Cast<ATabiCharacterBase>(OtherActor);
	if (!Target) return;

	ATabiCharacterBase* Owner = GetOwner<ATabiCharacterBase>();
	if (!Owner) return;

	// Do nothing if this character is not hostile to Target
	if (Owner->GetTeamAttitudeTowards(*Target) != ETeamAttitude::Hostile) return;

	if (AlreadyHitCharacters.Contains(Target)) return;
	AlreadyHitCharacters.Add(Target);

	FTabiHitEvent HitEvent;
	HitEvent.AttackDefinition = CurrentAttack;
	HitEvent.Attacker = Owner;
	HitEvent.Victim = Target;
	HitEvent.Location = ComputeHitLocation(Target, OtherComp);
	HitEvent.Rotation = ComputeHitRotation(Target);
	HitEvent.Result = Attack(Target);
	HitEvent.bFirstConfirmedHit = HitEvent.Result == ETabiHitResult::Damaged
		&& CurrentRequestID.IsValid()
		&& !FirstHitRequestID.IsEquivalent(CurrentRequestID);

	if (HitEvent.bFirstConfirmedHit)
	{
		FirstHitRequestID = CurrentRequestID;
	}

	OnTabiHitConfirmed.Broadcast(HitEvent);
}

FVector UTabiCombatComponent::ComputeHitLocation(const AActor* Target, const UPrimitiveComponent* TargetComp) const
{
	const AActor* Owner = GetOwner();
	const FVector HitboxCenter = Hitbox ? Hitbox->GetComponentLocation() : (Owner ? Owner->GetActorLocation() : FVector::ZeroVector);
	if (!Target) return HitboxCenter;

	FVector HitLocation = (HitboxCenter + Target->GetActorLocation()) * 0.5f;

	if (TargetComp)
	{
		FVector ClosestPoint;
		if (TargetComp->GetClosestPointOnCollision(HitboxCenter, ClosestPoint) >= 0.f)
		{
			HitLocation.X = ClosestPoint.X;
			HitLocation.Y = ClosestPoint.Y;
		}
	}

	return HitLocation;
}

FRotator UTabiCombatComponent::ComputeHitRotation(const AActor* Target) const
{
	const ATabiCharacterBase* Owner = GetOwner<ATabiCharacterBase>();

	FVector Direction = FVector::ZeroVector;
	if (Owner && Target)
	{
		Direction = Target->GetActorLocation() - Owner->GetActorLocation();
		Direction.Y = 0.f;
		Direction.Z = 0.f;
	}

	if (Direction.IsNearlyZero())
	{
		const bool bFacingRight = Owner ? Owner->IsFacingRight() : true;
		Direction = FVector(bFacingRight ? 1.f : -1.f, 0.f, 0.f);
	}

	return Direction.GetSafeNormal().Rotation();
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

ETabiHitResult UTabiCombatComponent::Attack(ATabiCharacterBase* Target)
{
	if (!Target) return ETabiHitResult::None;
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

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(StunEffectBlinkTimerHandle);
		World->GetTimerManager().ClearTimer(StunActivationTimerHandle);
	}

	Super::EndPlay(EndPlayReason);
}
