// Fill out your copyright notice in the Description page of Project Settings.


#include "Triggers/BorderOfRoom/BorderOfRoomBase.h"
#include "Net/UnrealNetwork.h"
#include "Components/ShapeComponent.h"

ABorderOfRoomBase::ABorderOfRoomBase()
{
	bReplicates = true;
	NetUpdateFrequency = 1.f;
	
	GetCollisionComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCollisionComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	AmountPlayerOfRoom = 0;
	bGhostInRoom = false;
}

void ABorderOfRoomBase::BeginPlay()
{
	Super::BeginPlay();
}

void ABorderOfRoomBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABorderOfRoomBase, bGhostInRoom);
	DOREPLIFETIME(ABorderOfRoomBase, AmountPlayerOfRoom);
}


void ABorderOfRoomBase::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	
}

void ABorderOfRoomBase::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}
