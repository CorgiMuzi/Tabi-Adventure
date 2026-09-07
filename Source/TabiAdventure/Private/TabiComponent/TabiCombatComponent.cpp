// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiComponent/TabiCombatComponent.h"

#include "TabiAnimation/TabiAnimInstance.h"
#include "TabiCharacter/TabiCharacterBase.h"
#include "TabiComponent/TabiStatComponent.h"
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

#if WITH_EDITOR
void UTabiCombatComponent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

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

bool UTabiCombatComponent::ApplyPoiseDamage(const float PoiseDamage)
{
	const ATabiCharacterBase* Owner = GetOwner<ATabiCharacterBase>();
	const UTabiStatComponent* StatComponent = Owner ? Owner->GetStatComponent() : nullptr;
	const float PoiseThreshold = StatComponent ? StatComponent->GetStatCurrentValue(ETabiStatType::Resistance) : 0.f;

	// Resistance 0 keeps the old behaviour: every hit staggers.
	if (PoiseThreshold <= 0.f) return true;

	const UWorld* World = GetWorld();
	const float Now = World ? World->GetTimeSeconds() : 0.f;

	if (Now - LastPoiseDamageTime > PoiseResetDelay) AccumulatedPoiseDamage = 0.f;
	LastPoiseDamageTime = Now;

	AccumulatedPoiseDamage += FMath::Max(PoiseDamage, 0.f);
	if (AccumulatedPoiseDamage < PoiseThreshold) return false;

	AccumulatedPoiseDamage = 0.f;
	return true;
}

void UTabiCombatComponent::OnCharacterDamaged(const UTabiAttackDefinition* AttackDefinition, AActor* DamagedActor, const AActor* DamageCauser)
{
	StopAttack();

	if (!DamagedActor) return;

	ATabiCharacterBase* DamagedCharacter = Cast<ATabiCharacterBase>(DamagedActor);
	if (!DamagedCharacter) return;

	DamagedCharacter->SetCharacterState(ETabiCharacterState::Stunned);

	// The stun clock belongs to the character that is stunned, not to the component that
	// observed the hit, so the duration rule and the timer stay together on ATabiCharacterBase.
	if (AttackDefinition)
	{
		DamagedCharacter->StartStunTimer(AttackDefinition->GetHitStunDuration());
	}
}

void UTabiCombatComponent::StartHitFlash()
{
	StartHitBlink(HitFlashDuration);
}

void UTabiCombatComponent::StartHitBlink(const float Duration)
{
	UWorld* World = GetWorld();
	ATabiCharacterBase* Owner = GetOwner<ATabiCharacterBase>();
	if (!World || !Owner || Duration <= 0.f) return;

	TWeakObjectPtr<UTabiCombatComponent> WeakThis(this);
	TWeakObjectPtr<ATabiCharacterBase> WeakOwner(Owner);
	
	Owner->SetSpriteColor(FLinearColor::Red);
	bFlashFlag = true;

	World->GetTimerManager().SetTimer(HitFlashIntervalTimerHandle, [WeakThis, WeakOwner]()
	{
		if (!WeakThis.IsValid() || !WeakOwner.IsValid()) return;

		WeakOwner->SetSpriteColor(WeakThis->bFlashFlag ? FLinearColor::Red : FLinearColor::White);
		WeakThis->bFlashFlag = !WeakThis->bFlashFlag;
	}, HitFlashInterval, true);

	World->GetTimerManager().SetTimer(HitFlashDurationTimerHandle, [WeakThis, WeakOwner]()
	{
		if (!WeakThis.IsValid()) return;

		if (const UWorld* InWorld = WeakThis->GetWorld())
		{
			InWorld->GetTimerManager().ClearTimer(WeakThis->HitFlashIntervalTimerHandle);
		}

		WeakThis->bFlashFlag = false;
		if (WeakOwner.IsValid()) WeakOwner->SetSpriteColor(FLinearColor::White);
	}, Duration, false);
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

FTabiRequestID UTabiCombatComponent::TryBeginAttack(const FTabiAttackContext& Context)
{
	const UTabiAttackDefinition* AttackDef = SelectAttackDefinition(Context);
	if (AttackDef == nullptr) return FTabiRequestID(0);

	if (!AnimInstance.IsValid()) return FTabiRequestID(0);
	const bool IsAnimQueued = AnimInstance->PlayAttackAnimation(AttackDef->GetAnimSequence());
	if (!IsAnimQueued) return FTabiRequestID(0);

	StoreRequestID();
	CurrentAttack = AttackDef;
	CurrentTarget = Context.Target;

	return GetCurrentRequestID();
}

void UTabiCombatComponent::GatherUsableAttacks(const FTabiAttackContext& Context, TArray<const UTabiAttackDefinition*>& OutAttacks) const
{
	OutAttacks.Reset();

	for (const TObjectPtr<UTabiAttackDefinition>& AttackDef : AttackDefinitions)
	{
		if (!AttackDef) continue;
		if (!AttackDef->IsUsableAt(Context)) continue;
		if (!IsAttackReady(AttackDef)) continue;

		OutAttacks.Add(AttackDef);
	}
}

bool UTabiCombatComponent::HasUsableAttack(const FTabiAttackContext& Context) const
{
	for (const TObjectPtr<UTabiAttackDefinition>& AttackDef : AttackDefinitions)
	{
		if (AttackDef && AttackDef->IsUsableAt(Context) && IsAttackReady(AttackDef)) return true;
	}

	return false;
}

bool UTabiCombatComponent::IsAttackReady(const UTabiAttackDefinition* AttackDefinition) const
{
	if (!AttackDefinition) return false;

	const float* ReadyTime = AttackReadyTime.Find(AttackDefinition);
	if (!ReadyTime) return true;

	const UWorld* World = GetWorld();
	if (!World) return true;

	return World->GetTimeSeconds() >= *ReadyTime;
}

bool UTabiCombatComponent::GetAttackDistanceBand(const FTabiAttackContext& Context, float& OutMinRange, float& OutMaxRange) const
{
	FTabiAttackContext DistanceFreeContext = Context;

	bool bFound = false;
	float MinRange = TNumericLimits<float>::Max();
	float MaxRange = 0.f;

	for (const TObjectPtr<UTabiAttackDefinition>& AttackDef : AttackDefinitions)
	{
		if (!AttackDef) continue;
		
		DistanceFreeContext.DistanceToTarget = AttackDef->GetMinAttackRange();
		if (!AttackDef->IsUsableAt(DistanceFreeContext)) continue;

		MinRange = FMath::Min(MinRange, AttackDef->GetMinAttackRange());
		MaxRange = FMath::Max(MaxRange, AttackDef->GetMaxAttackRange());
		bFound = true;
	}

	if (!bFound) return false;

	OutMinRange = MinRange;
	OutMaxRange = MaxRange;
	return true;
}

const UTabiAttackDefinition* UTabiCombatComponent::SelectAttackDefinition(const FTabiAttackContext& Context)
{
	// TODO: Make combo attack system. Don't select next attack definition randomly.
	TArray<const UTabiAttackDefinition*> UsableAttacks;
	GatherUsableAttacks(Context, UsableAttacks);

	if (UsableAttacks.IsEmpty()) return nullptr;

	return UsableAttacks[FMath::RandRange(0, UsableAttacks.Num() - 1)];
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
	
	if (CurrentAttack && CurrentAttack->GetCooldown() > 0.f)
	{
		if (const UWorld* World = GetWorld())
		{
			AttackReadyTime.Add(CurrentAttack, World->GetTimeSeconds() + CurrentAttack->GetCooldown());
		}
	}

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
		World->GetTimerManager().ClearTimer(HitFlashIntervalTimerHandle);
		World->GetTimerManager().ClearTimer(HitFlashDurationTimerHandle);
	}

	Super::EndPlay(EndPlayReason);
}
