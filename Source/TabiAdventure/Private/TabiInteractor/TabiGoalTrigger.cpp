// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiInteractor/TabiGoalTrigger.h"
#include "Components/BoxComponent.h"
#include "TabiCharacter/TabiPlayerCharacter.h"
#include "TabiGameMode/TabiAdventureGameModeBase.h"

ATabiGoalTrigger::ATabiGoalTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	GoalBoundary = CreateDefaultSubobject<UBoxComponent>(FName("GoalBoundary"));
	GoalBoundary->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnGoalBoundaryBeginOverlap);
}

void ATabiGoalTrigger::OnGoalBoundaryBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const ATabiPlayerCharacter* PlayerCharacter = Cast<ATabiPlayerCharacter>(OtherActor);
	if (!PlayerCharacter || !PlayerCharacter->IsLocallyControlled()) return;

	if (ATabiAdventureGameModeBase* TAGameMode = GetWorld()->GetAuthGameMode<ATabiAdventureGameModeBase>())
	{
		TAGameMode->OnPlayerReachedGoal();
	}
}
