// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiCharacter/TabiPlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "TabiAnimation/TabiAnimInstance.h"

#include "TabiComponent/TabiCombatComponent.h"
#include "TabiInteractor/TabiInteractable.h"

ATabiPlayerCharacter::ATabiPlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GetCharacterMovement()->bOrientRotationToMovement = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ATabiPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetTabiTeamId(ETabiCharacterTeamID::Player);
}

void ATabiPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
	EnhancedInput->BindAction(DodgeAction, ETriggerEvent::Started, this, &ThisClass::Dodge);
	EnhancedInput->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ThisClass::HandleAttackInput);
	EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::Interact);
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

void ATabiPlayerCharacter::Move(const FInputActionValue& Value)
{
	if (!CanMove()) return;

	const float ScaleX = FMath::Clamp(Value.Get<FVector2D>().X, -1.f, 1.f);

	MoveAlongX(ScaleX);
	/**
	* TODO: 3D Z-fighting problem.
	* const float ScaleY = FMath::Clamp(Value.Get<FVector2D>().Y, -1.f, 1.f);
	* MoveAlongY(ScaleY);
	**/

}

void ATabiPlayerCharacter::HandleAttackInput()
{
	RequestAttack();
}

void ATabiPlayerCharacter::Interact()
{
	TArray<AActor*> OverlappingActors;
	GetCapsuleComponent()->GetOverlappingActors(OverlappingActors);

	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (ITabiInteractable* Interactor = Cast<ITabiInteractable>(OverlappingActor))
		{
			Interactor->Interact(this);
			break;
		}
	}
}

void ATabiPlayerCharacter::HandleDeathAnimEnd()
{
	Super::HandleDeathAnimEnd();

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);	
}

void ATabiPlayerCharacter::SetGameplayInputEnabled(const bool bIsEnabled)
{
	APlayerController* PC = GetController<APlayerController>();
	if (!PC) return;

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
	{
		if (bIsEnabled)
		{
			Subsystem->AddMappingContext(LocomotionContext, 0);
		}
		else
		{
			Subsystem->RemoveMappingContext(LocomotionContext);
		}
	}
}

void ATabiPlayerCharacter::HandleAttackAnimEnd(bool IsCompleted)
{
	AttackComboStack = 0;
	Super::HandleAttackAnimEnd(IsCompleted);
}

void ATabiPlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (TabiAnimInstance)
	{
		TabiAnimInstance->StopAllAnimationOverrides();
	}

	Super::EndPlay(EndPlayReason);
}
