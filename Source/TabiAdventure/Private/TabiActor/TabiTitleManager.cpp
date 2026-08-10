// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiActor/TabiTitleManager.h"

#include "Engine/DirectionalLight.h"
#include "Components/DirectionalLightComponent.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Curves/CurveLinearColor.h"
#include "Curves/CurveFloat.h"

ATabiTitleManager::ATabiTitleManager()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");

	Tiles.Reserve(TileNum);
	for (int32 TileIndex = 0; TileIndex < TileNum; ++TileIndex)
	{
		const FName TileName(*FString::Printf(TEXT("Tile_%d"), TileIndex));
		UChildActorComponent* Tile = CreateDefaultSubobject<UChildActorComponent>(TileName);
		Tile->SetupAttachment(RootComponent);
		Tiles.Add(Tile);
	}
}

void ATabiTitleManager::BeginPlay()
{
	Super::BeginPlay();

	ArrangeTiles();

	TimeOfDay = FMath::Fmod(StartTimeOfDay, 1.f);
	ApplySunRotation();
	ApplySunColorAndIntensity();
	ApplyFogColor();
}

void ATabiTitleManager::ArrangeTiles()
{
	for (UChildActorComponent* Tile : Tiles)
	{
		if (!Tile) continue;
		if (Tile->GetChildActorClass() != TileClass)
		{
			Tile->SetChildActorClass(TileClass);
		}

		if (TileClass && !Tile->GetChildActor())
		{
			Tile->CreateChildActor();
		}
	}

	TileWidth = ResolveTileWidth();
	if (TileWidth <= 0) return;

	TilePositionOffset = -(TileWidth - 70.f);

	for (int32 TileIndex = 0; TileIndex < Tiles.Num(); ++TileIndex)
	{
		if (UChildActorComponent* Tile = Tiles[TileIndex])
		{
			Tile->SetRelativeLocation(FVector(TilePositionOffset + TileIndex * (TileWidth - 70.f), 0.f, 0.f));
		}
	}
}

float ATabiTitleManager::ResolveTileWidth()
{
	if (Tiles.IsEmpty()) return 0.f;

	for (const UChildActorComponent* Tile : Tiles)
	{
		if (!Tile) continue;

		const AActor* TileActor = Tile->GetChildActor();
		if (!TileActor) continue;

		FVector Origin;
		FVector BoxExtent;

		TileActor->GetActorBounds(false, Origin, BoxExtent);

		return BoxExtent.X * 2.f;
	}

	return 0.f;
}

void ATabiTitleManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AdvanceTimeOfDay(DeltaSeconds);

	if (TileWidth <= 0.f || ScrollSpeed <= 0.f) return;
	ScrollTiles(DeltaSeconds);
}

void ATabiTitleManager::AdvanceTimeOfDay(const float InDeltaTime)
{
	if (!SunLight || DayLengthSeconds <= 0.f) return;

	TimeOfDay = FMath::Fmod(TimeOfDay + InDeltaTime / DayLengthSeconds, 1.f);
	ApplySunRotation();
	ApplySunColorAndIntensity();
	ApplyFogColor();
}

void ATabiTitleManager::ApplySunRotation() const
{
	if (!SunLight) return;

	const float Angle = 2.f * PI * TimeOfDay;

	const float Yaw = SunBaseYaw + FMath::Sin(Angle) * SunYawSwing;

	const float PitchAlpha = (1.f - FMath::Cos(Angle)) * 0.5f;
	const float Pitch = FMath::Lerp(SunPitchLow, SunPitchHigh, PitchAlpha);

	SunLight->SetActorRotation(FRotator(Pitch, Yaw, 0.f));
}

void ATabiTitleManager::ApplySunColorAndIntensity() const
{
	if (!SunLight) return;

	UDirectionalLightComponent* LightComponent = SunLight->GetComponent();
	if (!LightComponent) return;

	if (SunColorOverDay)
	{
		const FLinearColor Color = SunColorOverDay->GetLinearColorValue(TimeOfDay);
		LightComponent->SetLightColor(Color, false);
	}

	if (SunIntensityOverDay)
	{
		const float Intensity = SunIntensityOverDay->GetFloatValue(TimeOfDay);
		LightComponent->SetIntensity(Intensity);
	}
}

void ATabiTitleManager::ApplyFogColor() const
{
	if (!SkyFog || !FogColorOverDay) return;

	UExponentialHeightFogComponent* FogComponent = SkyFog->GetComponent();
	if (!FogComponent) return;

	const FLinearColor Color = FogColorOverDay->GetLinearColorValue(TimeOfDay);
	FogComponent->SetFogInscatteringColor(Color);
}

void ATabiTitleManager::ScrollTiles(const float InDeltaTime)
{
	if (Tiles.IsEmpty()) return;

	const FVector DeltaDistance = FVector(-ScrollSpeed * InDeltaTime, 0.f, 0.f);
	for (UChildActorComponent* Tile : Tiles)
	{
		if (!Tile) continue;
		Tile->AddRelativeLocation(DeltaDistance);
		if (Tile->GetRelativeLocation().X <= TilePositionOffset)
		{
			Tile->SetRelativeLocation(FVector(TilePositionOffset + (TileWidth - 70.f) * TileNum, 0.f, 0.f));
		}
	}
}