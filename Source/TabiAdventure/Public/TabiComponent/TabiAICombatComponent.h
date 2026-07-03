// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TabiAICombatComponent.generated.h"

class ATabiCharacterBase;
class UBoxComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TABIADVENTURE_API UTabiAICombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTabiAICombatComponent();

	virtual void BeginPlay() override;

	void Attack(ATabiCharacterBase* Target);

	UFUNCTION()
	void EnableHitCollision();

	UFUNCTION()
	void DisableHitCollision();

	inline float GetAttackRange() const { return AttackRange; }
	inline float GetMinChaseRadius() const { return MinChaseRadius; }
	inline float GetMaxChaseRadius() const { return MaxChaseRadius; }

protected:
	UFUNCTION()
	void OnHitboxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(EditAnywhere, meta=(ClampMin="0"))
	float AttackRange;

	UPROPERTY(EditAnywhere, meta=(ClampMin="0"))
	float MinChaseRadius;

	UPROPERTY(EditAnywhere, meta=(ClampMin="0"))
	float MaxChaseRadius;\

private:
	UPROPERTY()
	TObjectPtr<UBoxComponent> Hitbox;

	UPROPERTY()
	TArray<TObjectPtr<ATabiCharacterBase>> AlreadyHitCharacters;
};
