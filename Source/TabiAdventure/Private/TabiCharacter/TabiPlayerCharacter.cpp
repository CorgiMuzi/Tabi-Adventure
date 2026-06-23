// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiCharacter/TabiPlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TabiComponent/TabiStatComponent.h"

#include "TabiAnimation/TabiAnimInstance.h"

ATabiPlayerCharacter::ATabiPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	Camera->SetupAttachment(SpringArm);

	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	MovementComp->SetPlaneConstraintOrigin(FVector(0.f, 5.f, 0.f));
}

void ATabiPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
	EnhancedInput->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ThisClass::Attack);
}

void ATabiPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ATabiPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (APlayerController* PC = Cast<APlayerController>(NewController))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(LocomotionContext, 0);
		}
	}
}

void ATabiPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UCharacterMovementComponent* MoveComp = GetCharacterMovement();

	if (MoveComp->MovementMode != MOVE_Falling)
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

	if (TabiAnimInstance)
	{
		TabiAnimInstance->SetSpeed(FMath::Abs(GetVelocity().X));
		TabiAnimInstance->SetIsFalling(GetCharacterMovement()->IsFalling());
	}
}

void ATabiPlayerCharacter::Move(const FInputActionValue& Value)
{
	if (!IsCharacterMovable()) return;

	const float ScaleX = FMath::Clamp(Value.Get<FVector2D>().X, -1.f, 1.f);

	MoveAlongX(ScaleX);
}

void ATabiPlayerCharacter::Jump()
{
	CharacterState = ETabiCharacterState::Jumping;
	if (TabiAnimInstance)
	{
		TabiAnimInstance->StopAllAnimationOverrides();
	}
	Super::Jump();
}

void ATabiPlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	CharacterState = ETabiCharacterState::Idling;
}

void ATabiPlayerCharacter::Attack()
{
	if (CharacterState == ETabiCharacterState::Attacking ||
		CharacterState == ETabiCharacterState::Jumping) return;

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

void ATabiPlayerCharacter::HandleAttackAnimEnd()
{
	if (CharacterState != ETabiCharacterState::Attacking) return;

	AttackComboStack = 0;
	CharacterState = ETabiCharacterState::Idling;
}

void ATabiPlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
