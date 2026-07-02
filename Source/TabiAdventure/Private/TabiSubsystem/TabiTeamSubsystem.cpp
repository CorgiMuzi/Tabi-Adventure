// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiSubsystem/TabiTeamSubsystem.h"
#include "GenericTeamAgentInterface.h"

void UTabiTeamSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FGenericTeamId::SetAttitudeSolver(
		[](FGenericTeamId TeamA, FGenericTeamId TeamB)
		{
			if (TeamA == TeamB) return ETeamAttitude::Friendly;
			if (TeamA == FGenericTeamId::NoTeam ||
				TeamB == FGenericTeamId::NoTeam) return ETeamAttitude::Neutral;
			return ETeamAttitude::Hostile;
		});
}

