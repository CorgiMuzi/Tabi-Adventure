// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiSubsystem/TabiDepthSortSubsystem.h"
#include "TabiCharacter/TabiCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void UTabiDepthSortSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RegisteredActors.RemoveAll([](const TWeakObjectPtr<ATabiCharacterBase>& Target)
	{
		return !Target.IsValid();
	});

	RegisteredActors.Sort([](const TWeakObjectPtr<ATabiCharacterBase>& Lhs, const TWeakObjectPtr<ATabiCharacterBase>& Rhs)
	{
		/**
		 * Establishing front-and-back draw order to prevent `Z-fighting`
		 * between characters sharing the same Y plane.
		 *
		 * Camera looks along +Y (from -Y), so a smaller Y is drawn in front.
		 *
		 * Ordering priority:
		 * 0. Team: A player should always be drawn in front.
		 * 1. Foot height(Z): A character on a lower floor is drawn in front.
		 * 2. Horizontal position(X): A character on a left side is drawn in front.
		 * 3. Unique ID: Guarantees a total order even if there are characters having the same priority.
		 */
		if (Lhs->IsPlayerTeam() != Rhs->IsPlayerTeam())
		{
			return Lhs->IsPlayerTeam();
		}

		const FVector& LLocation = Lhs->GetActorLocation();
		const FVector& RLocation = Rhs->GetActorLocation();

		if (FMath::IsNearlyEqual(LLocation.Z, RLocation.Z))
		{
			if (FMath::IsNearlyEqual(LLocation.X, RLocation.X))
			{
				return Lhs->GetUniqueID() < Rhs->GetUniqueID();
			}

			return LLocation.X < RLocation.X;
		}

		return LLocation.Z < RLocation.Z;
	});

	for (int32 i = 0; i < RegisteredActors.Num(); ++i)
	{
		const float TargetY = DepthYBase - i * DepthYStep;

		if (FMath::IsNearlyEqual(RegisteredActors[i]->GetActorLocation().Y, TargetY)) continue;

		UCharacterMovementComponent* MoveComp = RegisteredActors[i]->GetCharacterMovement();
		if (MoveComp) MoveComp->SetPlaneConstraintOrigin(FVector(0.f, TargetY, 0.f));

		FVector TargetLocation = RegisteredActors[i]->GetActorLocation();
		TargetLocation.Y = TargetY;
		RegisteredActors[i]->SetActorLocation(TargetLocation, false);
	}
}

TStatId UTabiDepthSortSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UTabiDepthSortSubsystem, STATGROUP_Tickables)
}

void UTabiDepthSortSubsystem::RegisterActor(ATabiCharacterBase* Actor)
{
	if (!Actor) return;
	if (RegisteredActors.Contains(Actor)) return;

	RegisteredActors.Add(Actor);
}

void UTabiDepthSortSubsystem::UnregisterActor(ATabiCharacterBase* Actor)
{
	if (!Actor) return;
	if (!RegisteredActors.Contains(Actor)) return;

	RegisteredActors.Remove(Actor);
}

void UTabiDepthSortSubsystem::BeginDestroy()
{
	RegisteredActors.Empty();

	Super::BeginDestroy();
}