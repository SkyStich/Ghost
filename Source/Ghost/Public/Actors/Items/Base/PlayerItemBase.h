// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Items/Base/BaseItem.h"
#include "PlayerItemBase.generated.h"

/**
 * Base item for use and interaction of the player
 */
UCLASS(Abstract)
class GHOST_API APlayerItemBase : public ABaseItem
{
	GENERATED_BODY()

protected:
	
	UFUNCTION()
	virtual void OnRep_ItemUsed() {}

public:

	APlayerItemBase();

	virtual bool UseItemDirectly() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	UPROPERTY(ReplicatedUsing = OnRep_ItemUsed)
	bool bItemUsed;
	
	UPROPERTY(Replicated)
	bool bCanItemUse;
};
