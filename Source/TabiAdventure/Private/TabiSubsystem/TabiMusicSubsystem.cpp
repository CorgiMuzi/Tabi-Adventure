// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiSubsystem/TabiMusicSubsystem.h"

#include "Components/AudioComponent.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "TabiSubsystem/TabiGameFlowSettings.h"

bool UTabiMusicSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UTabiMusicSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	const UTabiGameFlowSettings* Settings = GetDefault<UTabiGameFlowSettings>();
	if (!Settings) return;

	const FString LevelName = UGameplayStatics::GetCurrentLevelName(&InWorld, true);

	PlayMusic(ResolveLevelMusic(LevelName, *Settings), Settings->MusicFadeInTime);

	const bool bIsAdventureLevel = LevelName == Settings->AdventureLevel.GetAssetName();
	if (bIsAdventureLevel && !Settings->DefeatMusic.IsNull())
	{
		DefeatMusicLoadHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(
			Settings->DefeatMusic.ToSoftObjectPath());
	}
}

void UTabiMusicSubsystem::Deinitialize()
{
	if (MusicComponent)
	{
		MusicComponent->Stop();
		MusicComponent = nullptr;
	}

	MusicLoadHandle.Reset();
	DefeatMusicLoadHandle.Reset();

	Super::Deinitialize();
}


TSoftObjectPtr<USoundBase> UTabiMusicSubsystem::ResolveLevelMusic(const FString& LevelName, const UTabiGameFlowSettings& Settings)
{
	if (LevelName == Settings.AdventureLevel.GetAssetName()) return Settings.AdventureMusic;
	if (LevelName == Settings.MenuLevel.GetAssetName()) return Settings.MenuMusic;
	if (LevelName == Settings.CreditLevel.GetAssetName()) return Settings.CreditMusic;

	return nullptr;
}

void UTabiMusicSubsystem::PlayDefeatMusic()
{
	const UTabiGameFlowSettings* Settings = GetDefault<UTabiGameFlowSettings>();
	if (!Settings) return;

	PlayMusic(Settings->DefeatMusic, Settings->MusicFadeInTime);
}

void UTabiMusicSubsystem::PlayMusic(const TSoftObjectPtr<USoundBase>& Music, float FadeInTime)
{
	if (Music.IsNull())
	{
		StopMusic();
		return;
	}

	if (USoundBase* LoadedMusic = Music.Get())
	{
		StartMusicComponent(LoadedMusic, FadeInTime);
		return;
	}

	const TSoftObjectPtr<USoundBase> MusicToPlay = Music;
	TWeakObjectPtr<UTabiMusicSubsystem> WeakThis(this);

	MusicLoadHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(
		MusicToPlay.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda([WeakThis, MusicToPlay, FadeInTime]()
		{
			if (!WeakThis.IsValid()) return;
			WeakThis->StartMusicComponent(MusicToPlay.Get(), FadeInTime);
		}));
}

void UTabiMusicSubsystem::StartMusicComponent(USoundBase* Music, float FadeInTime)
{
	if (!Music) return;

	UWorld* World = GetWorld();
	if (!World) return;

	StopMusic();

	MusicComponent = UGameplayStatics::CreateSound2D(
		World, Music,
		1.f, 1.f, 0.f,
		nullptr,
		false,
		false
	);

	if (!MusicComponent) return;

	if (FadeInTime > 0.f)
	{
		MusicComponent->FadeIn(FadeInTime, 1.f, 0.f);
	}
	else
	{
		MusicComponent->Play();
	}
}

void UTabiMusicSubsystem::StopMusic()
{
	if (!MusicComponent) return;

	const UTabiGameFlowSettings* Settings = GetDefault<UTabiGameFlowSettings>();
	const float FadeOutTime = Settings ? Settings->MusicFadeOutTime : 0.f;

	if (FadeOutTime > 0.f)
	{
		MusicComponent->bAutoDestroy = true;
		MusicComponent->FadeOut(FadeOutTime, 0.f);
	}
	else
	{
		MusicComponent->Stop();
	}

	MusicComponent = nullptr;
}
