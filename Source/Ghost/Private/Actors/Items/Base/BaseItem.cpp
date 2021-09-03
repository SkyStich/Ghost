// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Items/Base/BaseItem.h"
#include "Net/UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"
#include "Components/Player/StoragePlayerComponent.h"

ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = false;

	bStaticMeshReplicateMovement = true;
	bReplicates = true;
	IsInStorage = false;
	NetUpdateFrequency = 10.f;
	NetCullDistanceSquared = 3500.f;
	GetStaticMeshComponent()->SetSimulatePhysics(true);

	SetReplicatingMovement(true);
}

void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseItem, IsInStorage);
}

void ABaseItem::PlayerInteractionWithItem_Implementation(AGhostCharacter* Player)
{
	if(Player->GetStoragePlayerComponent()->AddItemToStorage(this))
	{
		IsInStorage = true;
		SetOwner(Player);
		OnRep_IsStorage();

		if(Player->GetStoragePlayerComponent()->GetPlayerCurrentItem() != this)
			SetActorLocation(FVector(99999.f));
	}
}

void ABaseItem::OnRep_IsStorage()
{
	if(IsInStorage)
	{
		AddToStorage();
	}
	else
	{
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetStaticMeshComponent()->SetSimulatePhysics(true);
	}
}

void ABaseItem::AddToStorage()
{
	GetStaticMeshComponent()->SetSimulatePhysics(false);
	GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseItem::RemoveFromStorage()
{
	IsInStorage = false;
	SetOwner(this);
	OnRep_IsStorage();
}

void ABaseItem::UseItemDirectlyPressed()
{
	UseItemDirectly();
	Server_UseItemDirectly();
}

void ABaseItem::Server_UseItemDirectly_Implementation()
{
	UseItemDirectly();
}
