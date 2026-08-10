// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TabiTitleManager.generated.h"

class ADirectionalLight;
class AExponentialHeightFog;
class UCurveLinearColor;
class UCurveFloat;

UCLASS()
class TABIADVENTURE_API ATabiTitleManager : public AActor
{
	GENERATED_BODY()

public:
	ATabiTitleManager();
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

	void ArrangeTiles();
	float ResolveTileWidth();
	void ScrollTiles(const float InDeltaTime);

	void AdvanceTimeOfDay(const float InDeltaTime);
	void ApplySunRotation() const;
	void ApplySunColorAndIntensity() const;
	void ApplyFogColor() const;

	UPROPERTY(EditAnywhere, Category="Tabi|Title")
	float ScrollSpeed{1.f};

	UPROPERTY()
	TArray<TObjectPtr<UChildActorComponent>> Tiles;

	UPROPERTY(EditDefaultsOnly, Category="Tabi|Title")
	TSubclassOf<AActor> TileClass;

	UPROPERTY(EditInstanceOnly, Category="Tabi|Title|Sky")
	TObjectPtr<ADirectionalLight> SunLight;

	UPROPERTY(EditAnywhere, Category="Tabi|Title|Sky", meta=(ClampMin="1"))
	float DayLengthSeconds{24.f};

	UPROPERTY(EditAnywhere, Category="Tabi|Title|Sky", meta=(ClampMin="0", ClampMax="1"))
	float StartTimeOfDay{0.5f};

	UPROPERTY(EditAnywhere, Category="Tabi|Title|Sky")
	float SunBaseYaw{0.f};

	UPROPERTY(EditAnywhere, Category="Tabi|Title|Sky", meta=(ClampMin="0", ClampMax="180"))
	float SunYawSwing{60.f};

	UPROPERTY(EditAnywhere, Category="Tabi|Title|Sky", meta=(ClampMin="-90", ClampMax="0"))
	float SunPitchHigh{-70.f};

	UPROPERTY(EditAnywhere, Category="Tabi|Title|Sky", meta=(ClampMin="-90", ClampMax="0"))
	float SunPitchLow{-15.f};

	UPROPERTY(EditAnywhere, Category="Tabi|Title|Sky")
	TObjectPtr<UCurveLinearColor> SunColorOverDay;

	UPROPERTY(EditAnywhere, Category="Tabi|Title|Sky")
	TObjectPtr<UCurveFloat> SunIntensityOverDay;

	UPROPERTY(EditInstanceOnly, Category="Tabi|Title|Sky")
	TObjectPtr<AExponentialHeightFog> SkyFog;

	UPROPERTY(EditAnywhere, Category="Tabi|Title|Sky")
	TObjectPtr<UCurveLinearColor> FogColorOverDay;

private:
	int32 TileNum{3};
	float TileWidth{0.f};
	float TilePositionOffset{0.f};

	float TimeOfDay{0.f};
};
