// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Items/Base/BaseItem.h"
#include "Net/UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"
#include "Components/Player/StoragePlayerComponent.h"

// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	NetUpdateFrequency = 10.f;
	NetCullDistanceSquared = 3500.f;

	bIsStorage = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StatiMesh"));
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetupAttachment(RootComponent);
}

void ABaseItem::BeginPlay()
{
	Super::BeginPlay();

}

void ABaseItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseItem, bIsStorage);
}

void ABaseItem::PlayerInteractionWithItem_Implementation(AGhostCharacter* Player)
{
	if(Player->GetStoragePlayerComponent()->AddItemToStorage(this))
	{
		bIsStorage = true;
		OnRep_IsStorage();
	}
}

void ABaseItem::OnRep_IsStorage()
{
	if(!bIsStorage) return;
	
	StaticMesh->SetSimulatePhysics(false);
	StaticMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	SetActorHiddenInGame(true);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}