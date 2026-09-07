// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiCharacter/TabiEnemyBase.h"

#include "TabiCharacter/TabiEnemyAIController.h"
#include "TabiComponent/TabiAICombatComponent.h"
#include "TabiConstant/TabiEnemyBlackboardKeys.h"
#include "TabiData/TabiAttackDefinition.h"

#include "TabiWidget/TabiVitalBar.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"

ATabiEnemyBase::ATabiEnemyBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UTabiAICombatComponent>(Super::TabiCombatComponentName))
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidgetComponent->SetupAttachment(RootComponent);
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	HealthBarWidgetComponent->SetRelativeLocation(FVector::UpVector * 50.f);
	HealthBarWidgetComponent->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
}

void ATabiEnemyBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetTabiTeamId(ETabiCharacterTeamID::Enemy);
	AICombatComponent = Cast<UTabiAICombatComponent>(CombatComponent);
}

void ATabiEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	InitHealthBar();
}

void ATabiEnemyBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

#if ENABLE_DRAW_DEBUG
	if (bDrawAttackRangeDebug) DrawAttackRangeDebug();
#endif
}

FTabiRequestID ATabiEnemyBase::RequestAttack(const AActor* Target)
{
	return Super::RequestAttack(Target);
}

void ATabiEnemyBase::DrawAttackRangeDebug() const
{
#if ENABLE_DRAW_DEBUG
	const UWorld* World = GetWorld();
	if (!World || !CombatComponent) return;

	const FVector Origin = GetActorLocation();
	const float BandBottom = -GetCharacterHalfSize().Y;
	const float BandTop = GetCharacterHalfSize().Y;

	// One colour per definition so overlapping bands stay readable.
	static const FColor RangeColors[] = { FColor::Green, FColor::Cyan, FColor::Magenta, FColor::Orange };
	int32 ColorIndex = 0;

	for (const TObjectPtr<UTabiAttackDefinition>& AttackDef : CombatComponent->GetAttackDefinitions())
	{
		if (!AttackDef) continue;

		const FColor Color = RangeColors[ColorIndex % UE_ARRAY_COUNT(RangeColors)];
		++ColorIndex;

		// Vertical markers on both sides: solid for the max range, dashed-looking short bar for the min range.
		for (const float Side : { -1.f, 1.f })
		{
			const float MaxX = Side * AttackDef->GetMaxAttackRange();
			DrawDebugLine(World, Origin + FVector(MaxX, 0.f, BandBottom), Origin + FVector(MaxX, 0.f, BandTop), Color, false, -1.f, 0, 2.f);

			if (AttackDef->GetMinAttackRange() > 0.f)
			{
				const float MinX = Side * AttackDef->GetMinAttackRange();
				DrawDebugLine(World, Origin + FVector(MinX, 0.f, BandBottom), Origin + FVector(MinX, 0.f, BandTop * 0.5f), FColor::Red, false, -1.f, 0, 2.f);
			}
		}
	}

	// Line to the current target plus the distance the rules actually compare against.
	const ATabiEnemyAIController* EnemyController = Cast<ATabiEnemyAIController>(GetController());
	const UBlackboardComponent* BB = EnemyController ? EnemyController->GetBlackboardComponent() : nullptr;
	const AActor* Target = BB ? Cast<AActor>(BB->GetValueAsObject(TabiEnemyBlackboardKey::Target)) : nullptr;
	if (!Target) return;

	const float Distance = FMath::Abs(Target->GetActorLocation().X - Origin.X);
	const bool bCanAttack = HasUsableAttackAgainst(Target);

	DrawDebugLine(World, Origin, Target->GetActorLocation(), bCanAttack ? FColor::Green : FColor::Red, false, -1.f, 0, 1.f);

	// Keyed by this actor so the message is replaced every frame instead of piling up.
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(static_cast<uint64>(GetUniqueID()), 0.f,
			bCanAttack ? FColor::Green : FColor::Red,
			FString::Printf(TEXT("%s  Dist %.0f  %s"), *GetName(), Distance, bCanAttack ? TEXT("CAN ATTACK") : TEXT("no attack")));
	}
#endif
}

void ATabiEnemyBase::InitHealthBar()
{
	if (!HealthBarWidgetComponent) return;
	UTabiVitalBar* HealthBar = Cast<UTabiVitalBar>(HealthBarWidgetComponent->GetUserWidgetObject());
	if (!HealthBar) return;
	HealthBar->SetOwningVital(GetVitalComponent());
}

void ATabiEnemyBase::HandleDeathAnimEnd()
{
	Super::HandleDeathAnimEnd();

	Destroy();
}

float ATabiEnemyBase::GetAttackRadius() const
{
	return AICombatComponent ? AICombatComponent->GetAttackRadius() : 0.f;
}

float ATabiEnemyBase::GetMinChaseHalfRadius() const
{
	return AICombatComponent ? AICombatComponent->GetMinChaseHalfRadius() : 0.f;
}

float ATabiEnemyBase::GetMaxChaseHalfRadius() const
{
	return AICombatComponent ? AICombatComponent->GetMaxChaseHalfRadius() : 0.f;
}
