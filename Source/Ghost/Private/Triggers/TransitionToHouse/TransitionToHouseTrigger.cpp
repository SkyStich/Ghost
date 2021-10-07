// Fill out your copyright notice in the Description page of Project Settings.


#include "Triggers/TransitionToHouse/TransitionToHouseTrigger.h"

#include "Components/ShapeComponent.h"
#include "GameState/Base/SessionGameStateBase.h"
#include "Kismet/GameplayStatics.h"

ATransitionToHouseTrigger::ATransitionToHouseTrigger()
{
	GetCollisionComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCollisionComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ATransitionToHouseTrigger::BeginPlay()
{
	Super::BeginPlay();

	if(GetLocalRole() != ROLE_Authority)
	{
		Destroy();
	}
	else
	{
		GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATransitionToHouseTrigger::OnComponentBeginOverlap);
		GetCollisionComponent()->OnComponentEndOverlap.AddDynamic(this, &ATransitionToHouseTrigger::OnComponentEndOverlap);
	}
}

void ATransitionToHouseTrigger::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	auto const GS = Cast<ASessionGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	if(GS)
	{
		GS->SetPlayersInHouse(true);
	}
}

void ATransitionToHouseTrigger::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	/** Chack num player in house */
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);
	if(OverlappingActors.Num() <= 0) return;
	
	auto const GS = Cast<ASessionGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	if(GS)
	{
		GS->SetPlayersInHouse(true);
	}
}

