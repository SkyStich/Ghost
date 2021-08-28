// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Player/StoragePlayerComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UStoragePlayerComponent::UStoragePlayerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicated(true);
}


// Called when the game starts
void UStoragePlayerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UStoragePlayerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UStoragePlayerComponent, Items, COND_OwnerOnly);
	DOREPLIFETIME(UStoragePlayerComponent, CurrentItem);
}

bool UStoragePlayerComponent::AddItemToStorage(ABaseItem* const ItemToAdd)
{
	if(GetOwnerRole() != ROLE_Authority) return false;
	if(Items.Num() >= 3) return false;

	Items.Add(ItemToAdd);

	if(!CurrentItem)
	{
		SetCurrentItem(ItemToAdd);
	}
	return true;
}

void UStoragePlayerComponent::SetCurrentItem(ABaseItem* const ItemToSet)
{
	CurrentItem = ItemToSet;
	OnRep_CurrentItem();
}

void UStoragePlayerComponent::OnRep_CurrentItem() const
{
	OnNewCurrentItem.Broadcast(CurrentItem);
}

bool UStoragePlayerComponent::RemoveItemFromStorage(ABaseItem* Item)
{
	Items.Remove(Item);
	CurrentItem = nullptr;
	return true;
}

bool UStoragePlayerComponent::DropItemFromStorage(ABaseItem* const ItemToRemove)
{
	if(!RemoveItemFromStorage(ItemToRemove)) return false;
	
	ItemToRemove->RemoveFromStorage();
	OnRemoveItem.Broadcast(ItemToRemove);
	return false;
}