// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDAnimNotifyState.h"
#include "TabiAnimNotifyState_PlayLoopSound.generated.h"

class USoundBase;
class UAudioComponent;

UCLASS()
class TABIADVENTURE_API UTabiAnimNotifyState_PlayLoopSound : public UPaperZDAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void OnNotifyBegin_Implementation(UPaperZDAnimInstance* OwningInstance) const override;
	virtual void OnNotifyEnd_Implementation(UPaperZDAnimInstance* OwningInstance) const override;
	virtual FName GetDisplayName_Implementation() const override;

protected:
	UPROPERTY(EditAnywhere, Category="Tabi|Sound")
	TObjectPtr<USoundBase> MetaSound;

	UPROPERTY(EditAnywhere, Category="Tabi|Sound")
	FName AttachName;

	UPROPERTY(EditAnywhere, Category="Tabi|Sound", meta=(ClampMin="0.0"))
	float VolumeMultiplier = 1.f;

	UPROPERTY(EditAnywhere, Category="Tabi|Sound", meta=(ClampMin="0.0"))
	float PitchMultiplier = 1.f;

	UPROPERTY(EditAnywhere, Category="Tabi|Sound", meta=(ClampMin="0.0"))
	float FadeOutDuration = 0.f;

	UPROPERTY(EditAnywhere, Category="Tabi|Sound")
	bool bFollow{true};

#if WITH_EDITORONLY_DATA
	UPROPERTY(Transient)
	mutable TWeakObjectPtr<UAudioComponent> PreviewAudioComponent;
#endif
};
