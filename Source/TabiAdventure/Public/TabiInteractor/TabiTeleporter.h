// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TabiInteractor.h"
#include "GameFramework/Actor.h"
#include "TabiTeleporter.generated.h"

class UBoxComponent;
class ATabiPlayerCharacter;
class UUserWidget;

UCLASS()
class TABIADVENTURE_API ATabiTeleporter : public AActor, public ITabiInteractor
{
	GENERATED_BODY()

public:
	ATabiTeleporter();

	virtual void Interact(AActor* Instigator) override;
	virtual void ShowInteractInfo(AActor* Instigator) override;
	virtual void HideInteractInfo(AActor* Instigator) override;

protected:
	UFUNCTION()
	void OnInteractBoundaryOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnInteractBoundaryOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void PerformTeleport();
	void HandleFadeInFinished();

	UPROPERTY(EditAnywhere, Category="Tabi|Teleporter")
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(EditInstanceOnly, Category="Tabi|Teleporter")
	TObjectPtr<AActor> Destination;

	UPROPERTY(EditAnywhere, Category="Tabi|Teleporter")
	TSubclassOf<UUserWidget> InteractPromptWidgetClass;

	UPROPERTY(EditAnywhere, Category="Tabi|Teleporter")
	float FadeOutDuration = 0.25f;

	UPROPERTY(EditAnywhere, Category="Tabi|Teleporter")
	float FadeInDuration = 0.25f;

	UPROPERTY(EditAnywhere, Category="Tabi|Teleporter")
	FLinearColor FadeColor = FLinearColor::Black;

private:
	TWeakObjectPtr<ATabiPlayerCharacter> ActorToTeleport;

	FTimerHandle FadeOutTimerHandle;
	FTimerHandle FadeInTimerHandle;
};
