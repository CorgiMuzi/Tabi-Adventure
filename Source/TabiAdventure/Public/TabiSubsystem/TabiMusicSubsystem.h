// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StreamableManager.h"
#include "Subsystems/WorldSubsystem.h"
#include "TabiMusicSubsystem.generated.h"

class UAudioComponent;
class USoundBase;
class UTabiGameFlowSettings;

UCLASS()
class TABIADVENTURE_API UTabiMusicSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	//~ UWorldSubsystem
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;
	//~ End UWorldSubsystem
	
	void PlayDefeatMusic();
	void StopMusic();

protected:
	//~ UWorldSubsystem
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;
	//~ End UWorldSubsystem
	
	void PlayMusic(const TSoftObjectPtr<USoundBase>& Music, float FadeInTime);
	void StartMusicComponent(USoundBase* Music, float FadeInTime);
	
	static TSoftObjectPtr<USoundBase> ResolveLevelMusic(const FString& LevelName, const UTabiGameFlowSettings& Settings);

	UPROPERTY(Transient)
	TObjectPtr<UAudioComponent> MusicComponent;
	
private:
	TSharedPtr<FStreamableHandle> MusicLoadHandle;
	TSharedPtr<FStreamableHandle> DefeatMusicLoadHandle;
};
