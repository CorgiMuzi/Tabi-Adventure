// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiCharacter/TabiCharacterBase.h"

#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "TabiComponent/TabiVitalComponent.h"
#include "TabiComponent/TabiStatComponent.h"
#include "TabiComponent/TabiCombatComponent.h"

#include "TabiAnimation/TabiAnimInstance.h"

#include "TabiData/TabiAttackDefinition.h"

#include "PaperFlipbookComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

FName ATabiCharacterBase::TabiCombatComponentName(TEXT("TabiCombatComponent"));

ATabiCharacterBase::ATabiCharacterBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("TabiPawn"));

	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	MovementComp->bConstrainToPlane = true;
	MovementComp->bSnapToPlaneAtStart = true;
	MovementComp->bOrientRotationToMovement = false;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	Hitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
	Hitbox->SetCollisionProfileName(TEXT("Hitbox"));
	Hitbox->SetupAttachment(GetCapsuleComponent());

	Hurtbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hurtbox"));
	Hurtbox->SetCollisionProfileName(TEXT("Hurtbox"));
	Hurtbox->SetupAttachment(GetCapsuleComponent());

	VitalComponent = CreateDefaultSubobject<UTabiVitalComponent>(TEXT("VitalComponent"));
	VitalComponent->OnTabiHPDepleted.AddDynamic(this, &ThisClass::OnCharacterDead);
	StatComponent = CreateDefaultSubobject<UTabiStatComponent>(TEXT("StatComponent"));
	CombatComponent = CreateDefaultSubobject<UTabiCombatComponent>(TabiCombatComponentName);

	PerceptionStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("PerceptionStimulSource"));
	PerceptionStimuliSource->RegisterForSense(UAISense_Sight::StaticClass());
}

void ATabiCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (VitalComponent) VitalComponent->FillVitalValues();
	if (StatComponent) StatComponent->FillStatValues();
}

void ATabiCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	HitboxBaseOffset = FVector(Hitbox->GetRelativeLocation().X, 0.f, 0.f);

	TabiAnimInstance = CastChecked<UTabiAnimInstance>(GetAnimInstance());
	TabiAnimInstance->OnAttackAnimEnd.AddDynamic(this, &ThisClass::HandleAttackAnimEnd);
	TabiAnimInstance->OnDeathAnimEnd.BindDynamic(this, &ThisClass::HandleDeathAnimEnd);

	StatComponent->OnStatCurrentValueChanged.AddDynamic(this, &ThisClass::HandleSpeedChanged);

	Flipbook = GetSprite();
	DefaultColor = Flipbook->GetSpriteColor();
}

void ATabiCharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (TabiAnimInstance)
	{
		TabiAnimInstance->SetSpeed(FMath::Abs(GetVelocity().X));
		TabiAnimInstance->SetIsFalling(GetCharacterMovement()->IsFalling());
	}
}

FTabiRequestID ATabiCharacterBase::RequestAttack()
{
	if (CharacterState == ETabiCharacterState::Attacking ||
		CharacterState == ETabiCharacterState::Jumping ||
		CharacterState == ETabiCharacterState::Dead) return FTabiRequestID(0);

	FTabiRequestID AttackRequestID = CombatComponent->TryBeginAttack();
	if (AttackRequestID.IsValid()) CharacterState = ETabiCharacterState::Attacking;
	return AttackRequestID;
}

bool ATabiCharacterBase::ReceiveDamage(const UTabiAttackDefinition* AttackDefinition, const AActor* DamageCauser)
{
	// Return when failed to dealing damage.
	if (!VitalComponent || !VitalComponent->ReceiveDamage(AttackDefinition->GetDamage())) return false;
	// Don't play hit reaction animations when character is dead.
	if (CharacterState == ETabiCharacterState::Dead) return false;

	if (Flipbook)
	{
		Flipbook->SetSpriteColor(FLinearColor(1.f, 0.3f, 0.3f));
	}

	GetWorld()->GetTimerManager().SetTimer(HurtEffectTimerHandle, FTimerDelegate::CreateLambda(
		[this]()
		{
			if (Flipbook) Flipbook->SetSpriteColor(DefaultColor);
		}), .1f, false);

	if (DamageCauser)
	{
		FVector KnockbackDir = GetActorLocation() - DamageCauser->GetActorLocation();
		KnockbackDir.Z = 0.f;
		KnockbackDir.Y = 0.f;

		KnockbackDir = KnockbackDir.GetSafeNormal();

		FVector KnockbackVelocity = KnockbackDir * (AttackDefinition->GetKnockbackStrength() - StatComponent->GetStatCurrentValue(ETabiStatType::Resistance));
		KnockbackVelocity.Z = AttackDefinition->GetKnockbackLiftSpeed();

		LaunchCharacter(KnockbackVelocity, true, true);
	}

	return true;
}

void ATabiCharacterBase::SetTabiTeamId(const ETabiCharacterTeamID& TeamID)
{
	SetGenericTeamId(static_cast<uint8>(TeamID));
}

void ATabiCharacterBase::MoveAlongX(float ScaleX)
{
	if (FMath::IsNearlyZero(ScaleX)) return;

	SetFacingRight(ScaleX > 0.f);

	AddMovementInput(FVector::ForwardVector, ScaleX);
}

void ATabiCharacterBase::HandleSpeedChanged(ETabiStatType StatType, float NewSpeed, float OldSpeed)
{
	if (StatType != ETabiStatType::Speed) return;

	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

void ATabiCharacterBase::HandleAttackAnimEnd(bool IsCompleted)
{
	if (CharacterState != ETabiCharacterState::Attacking) return;
	CharacterState = ETabiCharacterState::Idling;
}

void ATabiCharacterBase::HandleDeathAnimEnd()
{
	Destroy();
}

void ATabiCharacterBase::OnCharacterDead()
{
	CharacterState = ETabiCharacterState::Dead;
	OnTabiCharacterDead.Broadcast();
	Hurtbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();

	PerceptionStimuliSource->UnregisterFromPerceptionSystem();

	if (!TabiAnimInstance) return;
	TabiAnimInstance->PlayDeadAnimation(DeadAnimSequence);
}

void ATabiCharacterBase::SetFacingRight(bool bNewFacingRight)
{
	if (bIsFacingRight == bNewFacingRight) return;
	bIsFacingRight = bNewFacingRight;
	OnFacingChanged();
}

bool ATabiCharacterBase::IsCharacterMovable() const
{
	return CharacterState != ETabiCharacterState::Attacking &&
		CharacterState != ETabiCharacterState::Dead;
}

bool ATabiCharacterBase::IsAlive() const
{
	return CharacterState != ETabiCharacterState::Dead;
}

void ATabiCharacterBase::OnFacingChanged()
{
	FRotator NewRot = GetSprite()->GetRelativeRotation();
	NewRot.Yaw = bIsFacingRight ? 0.f : 180.f;
	GetSprite()->SetRelativeRotation(NewRot);

	FVector Offset = HitboxBaseOffset;
	Offset.X = bIsFacingRight ? Offset.X : -Offset.X;
	Hitbox->SetRelativeLocation(Offset);
}
