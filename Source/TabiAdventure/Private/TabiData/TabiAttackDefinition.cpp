// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiData/TabiAttackDefinition.h"

#include "TabiActor/TabiProjectile.h"

bool UTabiAttackDefinition::IsRanged() const
{
	return ProjectileClass != nullptr;
}

bool UTabiAttackDefinition::IsUsableAt(const FTabiAttackContext& Context) const
{
	if (!AnimSequence) return false;
	
	if (!Context.bHasTarget) return true;

	if (bRequireSamePlatform && !Context.bSamePlatform) return false;

	if (Context.DistanceToTarget < MinAttackRange) return false;
	if (Context.DistanceToTarget > MaxAttackRange) return false;

	return true;
}
