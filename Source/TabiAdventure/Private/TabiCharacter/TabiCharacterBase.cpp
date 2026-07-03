// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiCharacter/TabiCharacterBase.h"

#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "TabiComponent/TabiVitalComponent.h"
#include "TabiComponent/TabiStatComponent.h"

#include "TabiAnimation/TabiAnimInstance.h"

#include "TabiData/TabiAttackDefinition.h"

#include "TabiGameFramework/TabiCollisionChannel.h"

#include "PaperFlipbookComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

ATabiCharacterBase::ATabiCharacterBase()
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
	Hitbox->SetCollisionResponseToAllChannels(ECR_Ignore);
	Hitbox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Hitbox->SetupAttachment(GetCapsuleComponent());

	Hurtbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hurtbox"));
	Hurtbox->SetCollisionObjectType(TABI_TRACE_HURTBOX);
	Hurtbox->SetCollisionResponseToAllChannels(ECR_Ignore);
	Hurtbox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Hurtbox->SetupAttachment(GetCapsuleComponent());

	VitalComponent = CreateDefaultSubobject<UTabiVitalComponent>(TEXT("VitalComponent"));
	VitalComponent->OnTabiHPDepleted.AddDynamic(this, &ThisClass::OnCharacterDead);
	StatComponent = CreateDefaultSubobject<UTabiStatComponent>(TEXT("StatComponent"));

	PerceptionStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("PerceptionStimulSource"));
	PerceptionStimuliSource->RegisterForSense(UAISense_Sight::StaticClass());
}

void ATabiCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (VitalComponent) VitalComponent->FillVitalValues();
}

void ATabiCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	HitboxBaseOffset = FVector(Hitbox->GetRelativeLocation().X, 0.f, 0.f);

	TabiAnimInstance = CastChecked<UTabiAnimInstance>(GetAnimInstance());
	TabiAnimInstance->OnAttackAnimEnd.BindDynamic(this, &ThisClass::HandleAttackAnimEnd);
	TabiAnimInstance->OnDeathAnimEnd.BindDynamic(this, &ThisClass::HandleDeathAnimEnd);

	StatComponent->OnStatCurrentValueChanged.AddDynamic(this, &ThisClass::HandleSpeedChanged);

	Flipbook = GetSprite();
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

void ATabiCharacterBase::Attack()
{
	if (CharacterState == ETabiCharacterState::Attacking ||
		CharacterState == ETabiCharacterState::Jumping ||
		CharacterState == ETabiCharacterState::Dead) return;

	if (!TabiAnimInstance) return;

	CharacterState = ETabiCharacterState::Attacking;

	// Uncomment the below codes when implementing combo attack system.
	/*
	 * UTabiAttackDefinition* AttackDef = AttackDefinitions[AttackComboStack++];
	 * if (AttackComboStack >= AttackDefinitions.Num()) return; AttackComboStack = 0;
	*/
	UTabiAttackDefinition* AttackDef = AttackDefinitions[FMath::RandRange(0, AttackDefinitions.Num()-1)];
	TabiAnimInstance->PlayAttackAnimation(AttackDef);
}

void ATabiCharacterBase::ReceiveDamage(float Damage, const AActor* DamageCauser)
{
	// Return when failed to dealing damage.
	if (!VitalComponent || !VitalComponent->ReceiveDamage(Damage)) return;
	// Don't play hit reaction animations when character is dead.
	if (CharacterState == ETabiCharacterState::Dead) return;

	if (Flipbook)
	{
		DefaultColor = Flipbook->GetSpriteColor();
		Flipbook->SetSpriteColor(FLinearColor(1.f, 0.3f, 0.3f));
	}

	GetWorld()->GetTimerManager().SetTimer(HurtEffectTimerHandle, FTimerDelegate::CreateLambda(
		[this]()
		{
			if (Flipbook) Flipbook->SetSpriteColor(DefaultColor);
		}), .1f, false);

	if (!DamageCauser) return;

	FVector KnockbackDir = GetActorLocation() - DamageCauser->GetActorLocation();
	KnockbackDir.Z = 0.f;
	KnockbackDir.Y = 0.f;

	KnockbackDir = KnockbackDir.GetSafeNormal();

	FVector KnockbackVelocity = KnockbackDir * KnockbackStrength;
	KnockbackVelocity.Z = KnockbackLiftSpeed;

	LaunchCharacter(KnockbackVelocity, true, true);
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

void ATabiCharacterBase::HandleAttackAnimEnd()
{
	if (CharacterState != ETabiCharacterState::Attacking) return;

	CharacterState = ETabiCharacterState::Idling;
}

void ATabiCharacterBase::HandleDeathAnimEnd()
{
	/* FIXME:
	 *	CharacterState becomes dead state when vital components notify that character's HP is 0.
	 *	CharacterBase catch that notify and change the state to dead but death animation could be playing at that moment.
	 */
	Destroy();
}

void ATabiCharacterBase::OnCharacterDead()
{
	CharacterState = ETabiCharacterState::Dead;
	OnTabiCharacterDead.Broadcast();
	Hurtbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();

	if (!TabiAnimInstance) return;
	TabiAnimInstance->PlayDeadAnimation();
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
