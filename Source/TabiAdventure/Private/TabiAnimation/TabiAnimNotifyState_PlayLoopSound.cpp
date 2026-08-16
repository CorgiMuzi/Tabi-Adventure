// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiAnimation/TabiAnimNotifyState_PlayLoopSound.h"

#include "Kismet/GameplayStatics.h"
#include "TabiAnimation/TabiAnimInstance.h"
#include "Components/AudioComponent.h"

void UTabiAnimNotifyState_PlayLoopSound::OnNotifyBegin_Implementation(UPaperZDAnimInstance* OwningInstance) const
{
	Super::OnNotifyBegin_Implementation(OwningInstance);

	if (!MetaSound || !SequenceRenderComponent.IsValid()) return;

	if (MetaSound->IsOneShot())
	{
		UE_LOG(LogAudio, Warning, TEXT("PlayLoopSound notify state: Anim %s tried to spawn one shot sound asset %s. Spawning suppressed"),
			   *GetName(), *MetaSound->GetName());
		return;
	}

#if WITH_EDITORONLY_DATA
	UWorld* World = GetWorld();
	if (World && World->WorldType == EWorldType::EditorPreview)
	{
		PreviewAudioComponent = UGameplayStatics::SpawnSound2D(World, MetaSound, VolumeMultiplier, PitchMultiplier);
	}
	else
#endif
	{
		UTabiAnimInstance* AnimInstance = Cast<UTabiAnimInstance>(OwningInstance);
		if (!AnimInstance) return;

		UAudioComponent* AudioComponent;
		if (bFollow)
		{
			AudioComponent = UGameplayStatics::SpawnSoundAttached(
				MetaSound,
				SequenceRenderComponent.Get(),
				AttachName,
				FVector::ZeroVector,
				EAttachLocation::SnapToTarget,
				true,
				VolumeMultiplier,
				PitchMultiplier
			);
		}
		else
		{
			AudioComponent = UGameplayStatics::SpawnSoundAtLocation(
				GetWorld(),
				MetaSound,
				SequenceRenderComponent->GetComponentLocation(),
				SequenceRenderComponent->GetComponentRotation(),
				VolumeMultiplier,
				PitchMultiplier
			);
		}

		if (!AudioComponent) return;
		AnimInstance->RegisterLoopSound(this, AudioComponent);
	}
}

void UTabiAnimNotifyState_PlayLoopSound::OnNotifyEnd_Implementation(UPaperZDAnimInstance* OwningInstance) const
{
	Super::OnNotifyEnd_Implementation(OwningInstance);

#if WITH_EDITORONLY_DATA
	if (PreviewAudioComponent.IsValid() && GetWorld()->WorldType == EWorldType::EditorPreview)
	{
		if (FadeOutDuration > 0.f) PreviewAudioComponent->FadeOut(FadeOutDuration, 0.f);
		else PreviewAudioComponent->Stop();
	}
#endif
	if (UTabiAnimInstance* AnimInstance = Cast<UTabiAnimInstance>(OwningInstance))
	{
		AnimInstance->StopLoopSound(this, FadeOutDuration);
	}
}

FName UTabiAnimNotifyState_PlayLoopSound::GetDisplayName_Implementation() const
{
	return MetaSound ? MetaSound->GetFName() : Super::GetDisplayName_Implementation();
}
