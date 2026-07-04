// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TabiCombatComponent.generated.h"

class UPaperFlipbookComponent;
class UTabiAttackDefinition;
class ATabiCharacterBase;
class UBoxComponent;

UCLASS(ClassGroup=(Tabi), meta=(BlueprintSpawnableComponent))
class TABIADVENTURE_API UTabiCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTabiCombatComponent();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void Attack(ATabiCharacterBase* Target/*, const UTabiAttackDefinition* AttackDefinition*/);

	UFUNCTION()
	void EnableHitCollision();

	UFUNCTION()
	void DisableHitCollision();

protected:
	UFUNCTION()
	void OnHitboxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY()
	TObjectPtr<UBoxComponent> Hitbox;

private:
	void SetupHitbox(ATabiCharacterBase* Owner);

	UPROPERTY()
	TArray<TObjectPtr<ATabiCharacterBase>> AlreadyHitCharacters;
};
