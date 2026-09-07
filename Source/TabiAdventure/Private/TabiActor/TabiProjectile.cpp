// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiActor/TabiProjectile.h"

#include "NiagaraFunctionLibrary.h"
#include "TabiCharacter/TabiCharacterBase.h"

#include "Components/SphereComponent.h"

#include "GameFramework/ProjectileMovementComponent.h"

#include "PaperFlipbookComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TabiConstant/TabiCollisionChannel.h"
#include "TabiData/TabiAttackDefinition.h"

ATabiProjectile::ATabiProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SetRootComponent(SphereComponent);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::HandleProjectileOverlap);
	SphereComponent->SetCollisionResponseToChannel(TABI_OBJECT_HURTBOX, ECollisionResponse::ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Block);
	
	ProjectileSprite = CreateDefaultSubobject<UPaperFlipbookComponent>("ProjectileSprite");
	ProjectileSprite->SetupAttachment(SphereComponent);
	ProjectileSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->bRotationFollowsVelocity = false;
	ProjectileMovement->InitialSpeed = 0.f;
	
	ProjectileMovement->bConstrainToPlane = true;
	ProjectileMovement->SetPlaneConstraintNormal(FVector::RightVector);
	
	ProjectileMovement->OnProjectileStop.AddDynamic(this, &ThisClass::HandleProjectileStop);
}

void ATabiProjectile::Fire(const ATabiCharacterBase* InShooter, const FVector& InDirection, const UTabiAttackDefinition* InAttackDefinition)
{
	if (!InShooter || !InAttackDefinition) return;
	
	ShooterTeamId = InShooter->GetGenericTeamId();
	AttackDefinition = InAttackDefinition;
	
	FRotator NewRot = ProjectileSprite->GetRelativeRotation();
	NewRot.Yaw = InDirection.X >= 0 ? 0.f : 180.f;
	ProjectileSprite->SetRelativeRotation(NewRot); 
	ProjectileMovement->Velocity = InDirection * InAttackDefinition->GetProjectileSpeed();
	SetLifeSpan(InAttackDefinition->GetProjectileLifeTime());
}

void ATabiProjectile::HandleProjectileStop(const FHitResult& ImpactResult)
{
	Destroy();
}

void ATabiProjectile::HandleProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ATabiCharacterBase* OtherCharacter = Cast<ATabiCharacterBase>(OtherActor))
	{
		if (FGenericTeamId::GetAttitude(ShooterTeamId, OtherCharacter->GetGenericTeamId()) == ETeamAttitude::Hostile)
		{
			const ETabiHitResult HitResult = OtherCharacter->ReceiveDamage(AttackDefinition, this);
			if (HitResult == ETabiHitResult::Damaged)
			{
				const FVector EffectLocation = (bFromSweep ? static_cast<FVector>(SweepResult.ImpactPoint) : GetActorLocation());
				
				if (USoundBase* HitSound = AttackDefinition->GetProjectileHitSound())
				{
					UGameplayStatics::PlaySoundAtLocation(
						this,
						HitSound, 
						EffectLocation,
						FRotator::ZeroRotator);	
				}else
				{
					UE_LOG(LogTemp, Warning, TEXT("Any hit sound assign to %s"), *GetName());
				}
				
				if (UNiagaraSystem* Particle = AttackDefinition->GetHitEffect())
				{
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					this,
					Particle,
					EffectLocation,
					ProjectileMovement->Velocity.Rotation(),
					AttackDefinition->GetHitEffectScale(),
					true
					);
				}else
				{
					UE_LOG(LogTemp, Warning, TEXT("Any niagara system assigned to %s"), *GetName());
				}
				
				Destroy();
			}
		}
	}
}

