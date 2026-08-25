// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiInteractor/TabiTeleporter.h"

#include "Components/BoxComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TabiCharacter/TabiPlayerCharacter.h"
#include "TabiCharacter/TabiPlayerController.h"
#include "Blueprint/UserWidget.h"

ATabiTeleporter::ATabiTeleporter()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(FName("BoxComponent"));
	RootComponent = BoxComponent;

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnInteractBoundaryOverlapBegin);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnInteractBoundaryOverlapEnd);
}

void ATabiTeleporter::OnInteractBoundaryOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATabiPlayerCharacter* PlayerCharacter = Cast<ATabiPlayerCharacter>(OtherActor);
	if (!PlayerCharacter || !PlayerCharacter->IsLocallyControlled()) return;

	ShowInteractInfo(PlayerCharacter);
}

void ATabiTeleporter::OnInteractBoundaryOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ATabiPlayerCharacter* PlayerCharacter = Cast<ATabiPlayerCharacter>(OtherActor);
	if (!PlayerCharacter || !PlayerCharacter->IsLocallyControlled()) return;

	HideInteractInfo(PlayerCharacter);
}

void ATabiTeleporter::ShowInteractInfo(APawn* Interactor)
{
	if (!Interactor) return;
	
	if (ATabiPlayerController* PlayerController = Interactor->GetController<ATabiPlayerController>())
	{
		InteractPromptWidget = PlayerController->AddWidgetToViewport(InteractPromptWidgetClass);
	}
}

void ATabiTeleporter::HideInteractInfo(APawn* Interactor)
{
	if (!Interactor) return;
	
	if (ATabiPlayerController* PlayerController = Interactor->GetController<ATabiPlayerController>())
	{
		PlayerController->RemoveWidgetFromViewport(InteractPromptWidget.Get());
	}
}

void ATabiTeleporter::Interact(APawn* Interactor)
{
	ATabiPlayerCharacter* PlayerCharacter = Cast<ATabiPlayerCharacter>(Interactor);
	if (!PlayerCharacter || !Destination) return;

	APlayerController* PlayerController = PlayerCharacter->GetController<APlayerController>();
	if (!PlayerController || !PlayerController->PlayerCameraManager) return;

	ActorToTeleport = PlayerCharacter;  

	PlayerCharacter->SetGameplayInputEnabled(false);
	PlayerCharacter->GetCharacterMovement()->StopMovementImmediately();

	PlayerController->PlayerCameraManager->StartCameraFade(0.f, 1.f, FadeOutDuration, FadeColor, false, true);

	GetWorldTimerManager().SetTimer(FadeOutTimerHandle, this, &ThisClass::PerformTeleport, FadeOutDuration, false);
}

void ATabiTeleporter::PerformTeleport()
{
	ATabiPlayerCharacter* PlayerCharacter = ActorToTeleport.Get();
	if (!PlayerCharacter || !Destination) return;

	PlayerCharacter->GetCharacterMovement()->StopMovementImmediately();

	PlayerCharacter->TeleportTo(Destination->GetActorLocation(), PlayerCharacter->GetActorRotation(), false, false);

	if (APlayerController* PlayerController = PlayerCharacter->GetController<APlayerController>())
	{
		if (PlayerController->PlayerCameraManager)
		{
			PlayerController->PlayerCameraManager->StartCameraFade(1.f, 0.f, FadeInDuration, FadeColor, false, true);
		}
	}

	GetWorldTimerManager().SetTimer(FadeInTimerHandle, this, &ThisClass::HandleFadeInFinished, FadeInDuration, false);
}

void ATabiTeleporter::HandleFadeInFinished()
{
	if (ATabiPlayerCharacter* PlayerCharacter = ActorToTeleport.Get())
	{
		PlayerCharacter->SetGameplayInputEnabled(true);
	}

	ActorToTeleport.Reset();
}
