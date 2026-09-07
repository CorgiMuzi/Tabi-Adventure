#pragma once

#include "CoreMinimal.h"

class AActor;
class UTabiAttackDefinition;

enum class ETabiHitResult : uint8
{
	None,
	Damaged,
	Evaded,
	Ignored
};

struct FTabiHitEvent
{
	ETabiHitResult Result = ETabiHitResult::None;
	const UTabiAttackDefinition* AttackDefinition = nullptr;
	AActor* Attacker = nullptr;
	AActor* Victim = nullptr;
	FVector Location = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;
	bool bFirstConfirmedHit = false;
};

struct FTabiAttackContext
{
	const AActor* Target = nullptr;
	bool bHasTarget = false;
	float DistanceToTarget = 0.f;
	bool bSamePlatform = false;
};

struct FTabiRequestID
{
private:
	static constexpr uint32 InvalidRequestID = 0;

	uint32 RequestID;

public:
	explicit FTabiRequestID(uint32 RequestID = InvalidRequestID) : RequestID(RequestID)
	{
	}

	inline bool IsEquivalent(uint32 OtherID) const
	{
		return OtherID != InvalidRequestID && this->IsValid() && RequestID == OtherID;
	}

	inline bool IsEquivalent(FTabiRequestID OtherID) const
	{
		return IsEquivalent(OtherID.RequestID);
	}

	inline bool IsValid() const
	{
		return RequestID != InvalidRequestID;
	}

	inline uint32 GetID() const { return RequestID; }

	void operator=(uint32 OtherID)
	{
		RequestID = OtherID;
	}

	operator uint32() const
	{
		return RequestID;
	}

	FString ToString() const
	{
		return FString::FromInt(int32(RequestID));
	}
};
