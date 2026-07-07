// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TabiData/TabiTypes.h"
#include "TabiCombatComponent.generated.h"

class UTabiAnimInstance;
class UPaperFlipbookComponent;
class UTabiAttackDefinition;
class ATabiCharacterBase;
class UBoxComponent;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTabiAttackEndSignature, const FTabiRequestID /*RequestID*/,bool /*bSuccess*/);

UCLASS(ClassGroup=(Tabi), meta=(BlueprintSpawnableComponent))
class TABIADVENTURE_API UTabiCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTabiCombatComponent();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual FTabiRequestID TryBeginAttack();
	virtual bool Attack(ATabiCharacterBase* Target/*, const UTabiAttackDefinition* AttackDefinition*/);

	UFUNCTION()
	void FinishAttack(bool IsCompleted);
	virtual void StopAttack();

	inline const UTabiAttackDefinition* GetCurrentAttack() const { return CurrentAttack; }

	UFUNCTION()
	virtual void EnableHitCollision();

	UFUNCTION()
	virtual void DisableHitCollision();

	inline float GetAttackHalfRadius() const { return AttackHalfRadius; }

	inline static uint32 GetNextRequestID() { return NextRequestID++;}
	inline FTabiRequestID GetCurrentRequestID() const { return CurrentRequestID; }
	inline void StoreRequestID() { CurrentRequestID = GetNextRequestID(); }

	FOnTabiAttackEndSignature OnTabiAttackEnd;
protected:
	//~ Animation
	UPROPERTY()
	TSoftObjectPtr<UTabiAnimInstance> AnimInstance;
	//~ End Animation

	//~ Hitbox
	UFUNCTION()
	virtual void OnHitboxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void SetupHitbox(ATabiCharacterBase* Owner);

	UPROPERTY()
	TObjectPtr<UBoxComponent> Hitbox;
	//~ End Hitbox

	UPROPERTY()
	TArray<TObjectPtr<AActor>> AlreadyHitCharacters;

	UPROPERTY(EditAnywhere, meta=(ClampMin="0"))
	float AttackHalfRadius = 20.f;

	/**
	 * Select which attack definition should character use when it handles attack.
	 * @return Selected attack definition
	 */
	virtual const UTabiAttackDefinition* SelectAttackDefinition();

	UPROPERTY(EditAnywhere, Category= "Tabi|Combat")
	TArray<TObjectPtr<UTabiAttackDefinition>> AttackDefinitions;

	UPROPERTY()
	TObjectPtr<const UTabiAttackDefinition> CurrentAttack;

private:
	static uint32 NextRequestID;
	FTabiRequestID CurrentRequestID;
};
