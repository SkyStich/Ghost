// Fill out your copyright notice in the Description page of Project Settings.


#include "Triggers/TransitionToHouse/TransitionToHouseTrigger.h"


#include "Components/BrushComponent.h"
#include "Components/ShapeComponent.h"
#include "GameState/Base/SessionGameStateBase.h"
#include "Kismet/GameplayStatics.h"

ATransitionToHouseTrigger::ATransitionToHouseTrigger()
{
	GetBrushComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetBrushComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
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
		GetBrushComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATransitionToHouseTrigger::OnComponentBeginOverlap);
		GetBrushComponent()->OnComponentEndOverlap.AddDynamic(this, &ATransitionToHouseTrigger::OnComponentEndOverlap);
	}
}

void ATransitionToHouseTrigger::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	auto const GS = Cast<ASessionGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	if(GS)
	{
		GS->SetPlayersInHouse(true);
		
#if WITH_EDITOR
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Players move to house"), true, true, FColor::Emerald, 2.f);
#endif
	}
}

void ATransitionToHouseTrigger::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	/** Chack num player in house */
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);
	if(OverlappingActors.Num() > 1) return;
	
	auto const GS = Cast<ASessionGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	if(GS)
	{
		GS->SetPlayersInHouse(false);
		
#if WITH_EDITOR
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Players leave from house"), true, true, FColor::Emerald, 2.f);
#endif
	}
}

