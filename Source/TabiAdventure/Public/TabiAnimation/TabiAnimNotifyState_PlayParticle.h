// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDAnimNotifyState.h"
#include "TabiAnimNotifyState_PlayParticle.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class TABIADVENTURE_API UTabiAnimNotifyState_PlayParticle : public UPaperZDAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void OnNotifyBegin_Implementation(UPaperZDAnimInstance* OwningInstance) const override;
	virtual void OnNotifyEnd_Implementation(UPaperZDAnimInstance* OwningInstance) const override;
	virtual FName GetDisplayName_Implementation() const override;

protected:
	UPROPERTY(EditAnywhere, Category="Tabi|Particle")
	TObjectPtr<UNiagaraSystem> ParticleEffect;

	UPROPERTY(EditAnywhere, Category="Tabi|Particle")
	FName AttachName;

	UPROPERTY(EditAnywhere, Category="Tabi|Particle")
	bool bFollow{true};

#if WITH_EDITORONLY_DATA
	UPROPERTY(Transient)
	mutable TWeakObjectPtr<UNiagaraComponent> PreviewParticleComponent;
#endif
};
