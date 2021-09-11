// Fill out your copyright notice in the Description page of Project Settings.


#include "Triggers/BorderOfRoom/BorderOfRoomTriggerVolume.h"

#include "Components/BrushComponent.h"
#include "Net/UnrealNetwork.h"

ABorderOfRoomTriggerVolume::ABorderOfRoomTriggerVolume()
{
	bReplicates = true;
	NetUpdateFrequency = 1;

	AmountPlayerOfRoom = 0;
	
	GetBrushComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetBrushComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ABorderOfRoomTriggerVolume::BeginPlay()
{
	Super::BeginPlay();

	if(GetLocalRole() == ROLE_Authority)
	{
		GetBrushComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABorderOfRoomTriggerVolume::OnComponentBeginOverlap);
		GetBrushComponent()->OnComponentEndOverlap.AddDynamic(this, &ABorderOfRoomTriggerVolume::OnComponentEndOverlap);
	}
}

void ABorderOfRoomTriggerVolume::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABorderOfRoomTriggerVolume, AmountPlayerOfRoom);
}

void ABorderOfRoomTriggerVolume::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	/** Add count the player in room if OtherActor has the Player tag */
	if(OtherActor->ActorHasTag("Player")) AmountPlayerOfRoom++;
	
	FString const Message = FString::Printf(TEXT("TEXT(CurrentPlayer: %d"), AmountPlayerOfRoom);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Purple, Message);
}

void ABorderOfRoomTriggerVolume::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	/** Decrease count the player in room if OtherActor has Player tag */
	if(OtherActor->ActorHasTag("Player")) AmountPlayerOfRoom--;
	
	FString const Message = FString::Printf(TEXT("TEXT(CurrentPlayer: %d"), AmountPlayerOfRoom);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Purple, Message);
}