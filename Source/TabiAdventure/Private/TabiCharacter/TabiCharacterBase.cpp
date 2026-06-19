// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiCharacter/TabiCharacterBase.h"

#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "TabiComponent/TabiVitalComponent.h"
#include "TabiComponent/TabiStatComponent.h"
#include "TabiComponent/TabiCombatComponent.h"

#include "TabiGameFramework/TabiCollisionChannel.h"

#include "PaperFlipbookComponent.h"

ATabiCharacterBase::ATabiCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("TabiPawn"));

	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	MovementComp->bConstrainToPlane = true;
	MovementComp->bSnapToPlaneAtStart = true;

	Hitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
	Hitbox->SetCollisionResponseToAllChannels(ECR_Ignore);
	Hitbox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Hitbox->SetupAttachment(GetCapsuleComponent());

	Hurtbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hurtbox"));
	Hurtbox->SetCollisionObjectType(TABI_TRACE_HURTBOX);
	Hurtbox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Hurtbox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Hurtbox->SetupAttachment(GetCapsuleComponent());

	VitalComponent = CreateDefaultSubobject<UTabiVitalComponent>(TEXT("VitalComponent"));
	StatComponent = CreateDefaultSubobject<UTabiStatComponent>(TEXT("StatComponent"));
	CombatComponent = CreateDefaultSubobject<UTabiCombatComponent>(TEXT("CombatComponent"));

}

void ATabiCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ATabiCharacterBase::ReceiveDamage(float Damage)
{
	if (VitalComponent)
	{
		VitalComponent->ReceiveDamage(Damage);
	}
}

void ATabiCharacterBase::SetFacingRight(bool bNewFacingRight)
{
	if (bIsFacingRight == bNewFacingRight) return;
	bIsFacingRight = bNewFacingRight;
	OnFacingChanged();
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