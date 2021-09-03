// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Items/Base/PlayerItemBase.h"
#include "Net/UnrealNetwork.h"

APlayerItemBase::APlayerItemBase()
{
	bItemUsed = false;
	bCanItemUse = true;
}

void APlayerItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerItemBase, bItemUsed);
	DOREPLIFETIME(APlayerItemBase, bCanItemUse);
}

bool APlayerItemBase::UseItemDirectly()
{
	return bCanItemUse ? true : false;
}

