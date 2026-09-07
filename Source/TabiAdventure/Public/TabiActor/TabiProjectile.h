// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Actor.h"
#include "TabiProjectile.generated.h"

class UProjectileMovementComponent;
class ATabiCharacterBase;
class USphereComponent;
class UPaperFlipbookComponent;
class UTabiAttackDefinition;

UCLASS()
class TABIADVENTURE_API ATabiProjectile : public AActor
{
	GENERATED_BODY()

public:
	ATabiProjectile();
	
	void Fire(const ATabiCharacterBase* InShooter, const FVector& InDirection, const UTabiAttackDefinition* InAttackDefinition);
	
protected:
	UFUNCTION()
	void HandleProjectileStop(const FHitResult& ImpactResult);
	
	UFUNCTION()
	void HandleProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> SphereComponent;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UPaperFlipbookComponent> ProjectileSprite;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<const UTabiAttackDefinition> AttackDefinition;
	
	FGenericTeamId ShooterTeamId;
};
