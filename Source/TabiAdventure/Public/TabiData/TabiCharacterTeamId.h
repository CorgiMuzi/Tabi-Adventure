// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.
#pragma once

UENUM(BlueprintType)
enum class ETabiCharacterTeamID : uint8
{
	Player = 0 UMETA(DisplayName = "Player"),
	Enemy = 1 UMETA(DisplayName = "Enemy"),

	MAX UMETA(Hidden)
};