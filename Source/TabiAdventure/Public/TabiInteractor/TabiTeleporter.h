// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TabiInteractable.h"
#include "GameFramework/Actor.h"
#include "TabiTeleporter.generated.h"

class UBoxComponent;
class ATabiPlayerCharacter;
class UUserWidget;

UCLASS()
class TABIADVENTURE_API ATabiTeleporter : public AActor, public ITabiInteractable
{
	GENERATED_BODY()

public:
	ATabiTeleporter();

	//~ ITabiInteractable
	virtual void Interact(APawn* Interactor) override;
	virtual void ShowInteractInfo(APawn* Interactor) override;
	virtual void HideInteractInfo(APawn* Interactor) override;
	//~ End ITabiInteractable

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
	TWeakObjectPtr<UUserWidget> InteractPromptWidget;

	FTimerHandle FadeOutTimerHandle;
	FTimerHandle FadeInTimerHandle;
};
