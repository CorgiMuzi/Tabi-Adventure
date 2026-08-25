// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiAnimation/TabiAnimNotifyState_PlayParticle.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "TabiAnimation/TabiAnimInstance.h"

void UTabiAnimNotifyState_PlayParticle::OnNotifyBegin_Implementation(UPaperZDAnimInstance* OwningInstance) const
{
	Super::OnNotifyBegin_Implementation(OwningInstance);

	if (!ParticleEffect || !SequenceRenderComponent.IsValid()) return;

#if WITH_EDITORONLY_DATA
	UWorld* World = GetWorld();
	if (World && World->WorldType == EWorldType::EditorPreview)
	{
		if (bFollow)
		{
			PreviewParticleComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
				ParticleEffect,
				SequenceRenderComponent.Get(),
				AttachName,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::SnapToTarget,
				/*bAutoDestroy*/ false
			);
		}
		else
		{
			PreviewParticleComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				World,
				ParticleEffect,
				SequenceRenderComponent->GetComponentLocation(),
				SequenceRenderComponent->GetComponentRotation(),
				FVector::OneVector,
				/*bAutoDestroy*/ false
			);
		}
	}
	else
#endif
	{
		UTabiAnimInstance* AnimInstance = Cast<UTabiAnimInstance>(OwningInstance);
		if (!AnimInstance) return;

		UNiagaraComponent* NiagaraComponent;
		if (bFollow)
		{
			NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
				ParticleEffect,
				SequenceRenderComponent.Get(),
				AttachName,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::SnapToTarget,
				/*bAutoDestroy*/ false
			);
		}
		else
		{
			NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				ParticleEffect,
				SequenceRenderComponent->GetComponentLocation(),
				SequenceRenderComponent->GetComponentRotation(),
				FVector::OneVector,
				/*bAutoDestroy*/ false
			);
		}

		if (!NiagaraComponent) return;
		AnimInstance->RegisterLoopParticle(this, NiagaraComponent);
	}
}

void UTabiAnimNotifyState_PlayParticle::OnNotifyEnd_Implementation(UPaperZDAnimInstance* OwningInstance) const
{
	Super::OnNotifyEnd_Implementation(OwningInstance);

#if WITH_EDITORONLY_DATA
	if (PreviewParticleComponent.IsValid() && GetWorld()->WorldType == EWorldType::EditorPreview)
	{
		PreviewParticleComponent->Deactivate();
	}
#endif
	if (UTabiAnimInstance* AnimInstance = Cast<UTabiAnimInstance>(OwningInstance))
	{
		AnimInstance->StopLoopParticle(this);
	}
}

FName UTabiAnimNotifyState_PlayParticle::GetDisplayName_Implementation() const
{
	return ParticleEffect ? ParticleEffect->GetFName() : Super::GetDisplayName_Implementation();
}
