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
	MovementComp->SetPlaneConstraintOrigin(FVector::ZeroVector);
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
	if (StatComponent)
	{
		StatComponent->FillStatValues();
		HandleSpeedChanged(ETabiStatType::Speed, StatComponent->GetStatCurrentValue(ETabiStatType::Speed), 0.f /*Dummy Value*/);
	}
}

void ATabiCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	TabiAnimInstance = CastChecked<UTabiAnimInstance>(GetAnimInstance());
	TabiAnimInstance->OnAttackAnimEnd.AddDynamic(this, &ThisClass::HandleAttackAnimEnd);
	TabiAnimInstance->OnDeathAnimEnd.BindDynamic(this, &ThisClass::HandleDeathAnimEnd);

	StatComponent->OnStatCurrentValueChanged.AddDynamic(this, &ThisClass::HandleSpeedChanged);

	Flipbook = GetSprite();
	DefaultColor = Flipbook->GetSpriteColor();

	CurrentPlatform = GetCharacterMovement()->CurrentFloor.HitResult.GetActor();
}

void ATabiCharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (TabiAnimInstance)
	{
		TabiAnimInstance->SetSpeed(FMath::Abs(GetVelocity().X));
		TabiAnimInstance->SetIsFalling(GetCharacterMovement()->IsFalling());
	}

	UCharacterMovementComponent* MoveComp = GetCharacterMovement();

	if (CurrentState == ETabiCharacterState::Dodging)
	{
		MoveComp->Velocity.X = DodgeDirection * DodgeSpeed;
	}
	else if (!MoveComp->IsFalling())
	{
		MoveComp->GravityScale = DefaultGravityScale;
	}
	else
	{
		float VelocityZ = MoveComp->Velocity.Z;

		if (VelocityZ > ApexVelocityThreshold)
		{
			// When Character starts jumping.
			MoveComp->GravityScale = AscendingGravityScale;
		}
		else if (VelocityZ < -ApexVelocityThreshold)
		{
			// When character falling after jumped.
			MoveComp->GravityScale = FallingGravityScale;
		}
		else
		{
			// The highest point of the character when it junped.
			MoveComp->GravityScale = ApexGravityScale;
		}
	}
}

bool ATabiCharacterBase::CanJumpInternal_Implementation() const
{
	return Super::CanJumpInternal_Implementation() &&
		CurrentState != ETabiCharacterState::Dead &&
		CurrentState != ETabiCharacterState::Attacking &&
		CurrentState != ETabiCharacterState::Stunned;
}

void ATabiCharacterBase::Jump()
{
	if (!CanJump()) return;
	SetCharacterState(ETabiCharacterState::Jumping);
	if (TabiAnimInstance)
	{
		TabiAnimInstance->StopAllAnimationOverrides();
	}

	Super::Jump();
}

void ATabiCharacterBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (bIsAirDodging) bIsAirDodging = false;
	SetCharacterState(ETabiCharacterState::Idling);
	CurrentPlatform = Hit.GetActor();
}

FTabiRequestID ATabiCharacterBase::RequestAttack()
{
	if (!CanAttack()) return FTabiRequestID(0);

	FTabiRequestID AttackRequestID = CombatComponent->TryBeginAttack();
	if (AttackRequestID.IsValid()) SetCharacterState(ETabiCharacterState::Attacking);
	return AttackRequestID;
}

bool ATabiCharacterBase::CanAttack() const
{
	return CurrentState != ETabiCharacterState::Attacking &&
		CurrentState != ETabiCharacterState::Dead &&
		CurrentState != ETabiCharacterState::Jumping &&
		CurrentState != ETabiCharacterState::Dodging &&
		CurrentState != ETabiCharacterState::Stunned;
}

void ATabiCharacterBase::StopAttack()
{
	if (CombatComponent) CombatComponent->StopAttack();
}

bool ATabiCharacterBase::ReceiveDamage(const UTabiAttackDefinition* AttackDefinition, const AActor* DamageCauser)
{
	// Return when failed to dealing damage.
	if (!VitalComponent || !VitalComponent->ReceiveDamage(AttackDefinition->GetDamage())) return false;
	// Don't play hit reaction animations when character is dead.
	if (CurrentState == ETabiCharacterState::Dead) return false;

	StopAttack();

	if (Flipbook)
	{
		Flipbook->SetSpriteColor(FLinearColor(1.f, 0.3f, 0.3f));
	}

	GetWorld()->GetTimerManager().SetTimer(HurtEffectTimerHandle, FTimerDelegate::CreateLambda(
		                                       [this]()
		                                       {
			                                       if (Flipbook) Flipbook->SetSpriteColor(DefaultColor);
		                                       }), .1f, false);

	if (DamageCauser && SetCharacterState(ETabiCharacterState::Stunned))
	{
		StartStunTimer(AttackDefinition->GetHitStunDuration());
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

bool ATabiCharacterBase::CanDodge() const
{
	return CanMove() && !bNeedRecoverToDodge;
}

void ATabiCharacterBase::Dodge()
{
	if (!CanDodge()) return;

	bNeedRecoverToDodge = true;

	GetWorldTimerManager().SetTimer(DodgeExecutionTimerHandle, [this]()
	{
		SetCharacterState(ETabiCharacterState::Dodging);
		DodgeDirection = IsFacingRight() ? 1.f : -1.f;
		// Character will be invunerable after 10% of DodgeDuration times.
		GetCharacterMovement()->IsFalling() ? ExecuteAirDodge() : ExecuteDodge();
	}, DodgeDuration * 0.1f, false);
}

void ATabiCharacterBase::ExecuteDodge()
{
	float GroundFriction = GetCharacterMovement()->GroundFriction;
	GetCharacterMovement()->GroundFriction = 0.f;

	float BrakingDecelerationWalking = GetCharacterMovement()->BrakingDecelerationWalking;
	GetCharacterMovement()->BrakingDecelerationWalking = 0.f;

	GetWorldTimerManager().SetTimer(DodgeInvulnerableTimerHandle, [this, GroundFriction, BrakingDecelerationWalking]()
	{
		EndDodge(GroundFriction, BrakingDecelerationWalking);
	}, DodgeDuration, false);
}

void ATabiCharacterBase::ExecuteAirDodge()
{
	if (bIsAirDodging) return;
	bIsAirDodging = true;
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	MoveComp->GravityScale = 0.f;
	MoveComp->Velocity.Z = 0.f;
	GetWorldTimerManager().SetTimer(DodgeInvulnerableTimerHandle, [this]()
	{
		EndAirDodge();
	}, DodgeDuration, false);
}

void ATabiCharacterBase::EndAirDodge()
{
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();

	SetCharacterState(ETabiCharacterState::Jumping);
	MoveComp->GravityScale = DefaultGravityScale;
	MoveComp->MaxWalkSpeed = StatComponent->GetStatCurrentValue(ETabiStatType::Speed);

	GetWorldTimerManager().SetTimer(DodgeRecoveryTimerHandle, [this]()
	{
		bNeedRecoverToDodge = false;
	}, DodgeRecoveryTime, false);
}

void ATabiCharacterBase::EndDodge(const float GroundFriction, const float BrakingDecelerationWalking)
{
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();

	SetCharacterState(ETabiCharacterState::Idling);

	MoveComp->GroundFriction = GroundFriction;
	MoveComp->BrakingDecelerationWalking = BrakingDecelerationWalking;
	MoveComp->MaxWalkSpeed = StatComponent->GetStatCurrentValue(ETabiStatType::Speed);

	GetWorldTimerManager().SetTimer(DodgeRecoveryTimerHandle, [this]()
	{
		bNeedRecoverToDodge = false;
	}, DodgeRecoveryTime, false);
}

void ATabiCharacterBase::StartStunTimer(float BaseStunDuration, bool ShouldApplyStat /*true*/)
{
	if (!CombatComponent) return;

	const float FinalStunDuration = BaseStunDuration;
	if (ShouldApplyStat)
	{
		/**
		 * Calculate final stun duration when stat should be applied.
		 */
	}

	if (FinalStunDuration <= 0.f) return;

	GetWorldTimerManager().SetTimer(StunnedTimerHandle, FTimerDelegate::CreateLambda(
		                                [this]()
		                                {
			                                SetCharacterState(ETabiCharacterState::Idling);
		                                }), FinalStunDuration, false);
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
	if (CurrentState != ETabiCharacterState::Attacking) return;
	SetCharacterState(ETabiCharacterState::Idling);
}

void ATabiCharacterBase::HandleDeathAnimEnd()
{
	Destroy();
}

void ATabiCharacterBase::OnCharacterDead()
{
	SetCharacterState(ETabiCharacterState::Dead);
	OnTabiCharacterDead.Broadcast();
}

void ATabiCharacterBase::FaceToward(const AActor* Target)
{
	if (!Target) return;

	const float XDiff = Target->GetActorLocation().X - GetActorLocation().X;
	if (FMath::IsNearlyZero(XDiff)) return;

	SetFacingRight(XDiff > 0.f);
}


void ATabiCharacterBase::SetFacingRight(bool bNewFacingRight)
{
	if (bIsFacingRight == bNewFacingRight) return;
	bIsFacingRight = bNewFacingRight;
	OnFacingChanged();
}

void ATabiCharacterBase::OnFacingChanged()
{
	// FVector NewScale = GetSprite()->GetRelativeScale3D();
	// NewScale.X = bIsFacingRight ? FMath::Abs(NewScale.X) : -FMath::Abs(NewScale.X);
	// GetSprite()->SetRelativeScale3D(NewScale);

	FRotator NewRot = GetSprite()->GetRelativeRotation();
	NewRot.Yaw = bIsFacingRight ? 0.f : 180.f;
	GetSprite()->SetRelativeRotation(NewRot);

	if (Hitbox)
	{
		FVector Offset = CombatComponent ? CombatComponent->GetHitboxBaseOffset() : GetCharacterHalfSize();
		Offset.X = bIsFacingRight ? Offset.X : -Offset.X;
		Hitbox->SetRelativeLocation(Offset);
	}
}

bool ATabiCharacterBase::SetCharacterState(ETabiCharacterState NewState)
{
	const ETabiCharacterState OldState = CurrentState;
	if (OldState == ETabiCharacterState::Dead) return false;
	if (OldState == NewState) return false;

	if (OldState == ETabiCharacterState::Stunned)
	{
		if (NewState != ETabiCharacterState::Dead && NewState != ETabiCharacterState::Idling) return false;
	}

	CurrentState = NewState;

	OnCharacterStateChanged(OldState, NewState);

	return true;
}

void ATabiCharacterBase::OnCharacterStateChanged(ETabiCharacterState OldState, ETabiCharacterState NewState)
{
	switch (OldState)
	{
		case ETabiCharacterState::Attacking:
			if (Hitbox) Hitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;

		case ETabiCharacterState::Dodging:
			if (VitalComponent) VitalComponent->SetVulnerability(true);
			break;

		default: break;
	}

	switch (NewState)
	{
		case ETabiCharacterState::Dead:
			if (Hurtbox) Hurtbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->StopMovementImmediately();
			GetCharacterMovement()->DisableMovement();

			PerceptionStimuliSource->UnregisterFromPerceptionSystem();

			if (TabiAnimInstance) TabiAnimInstance->PlayDeadAnimation(DeadAnimSequence);
			break;

		case ETabiCharacterState::Dodging:
			if (VitalComponent) VitalComponent->SetVulnerability(false);
			break;

		default: break;
	}
}

bool ATabiCharacterBase::CanMove() const
{
	return CurrentState != ETabiCharacterState::Attacking &&
		CurrentState != ETabiCharacterState::Dead &&
		CurrentState != ETabiCharacterState::Dodging &&
		CurrentState != ETabiCharacterState::Stunned;
}

bool ATabiCharacterBase::IsAlive() const
{
	return CurrentState != ETabiCharacterState::Dead;
}

const AActor* ATabiCharacterBase::GetCurrentPlatform() const
{
	const FFindFloorResult& Floor = GetCharacterMovement()->CurrentFloor;
	// When character is walking on valid platform
	if (Floor.IsWalkableFloor())
	{
		return Floor.HitResult.GetActor();
	}

	// Return cached platform when character is jumping/falling or on invalid platform
	return CurrentPlatform;
}

bool ATabiCharacterBase::IsOnSamePlatformAs(const AActor* OtherActor) const
{
	const ATabiCharacterBase* OtherCharacter = Cast<ATabiCharacterBase>(OtherActor);
	if (OtherCharacter == nullptr) return false;

	const AActor* MyPlatform = GetCurrentPlatform();
	const AActor* OtherPlatform = OtherCharacter->GetCurrentPlatform();
	return MyPlatform && MyPlatform == OtherPlatform;
}

FVector ATabiCharacterBase::GetCharacterHalfSize() const
{
	const float HalfWidth = Hurtbox ? Hurtbox->GetUnscaledBoxExtent().X : GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	const float HalfHeight = Hurtbox ? Hurtbox->GetUnscaledBoxExtent().Y : GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	const float HalfDepth = Hurtbox ? Hurtbox->GetUnscaledBoxExtent().Z : GetCapsuleComponent()->GetUnscaledCapsuleRadius();

	return FVector{HalfWidth, HalfHeight, HalfDepth};
}
