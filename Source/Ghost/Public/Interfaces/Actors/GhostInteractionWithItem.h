// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GhostInteractionWithItem.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGhostInteractionWithItem : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GHOST_API IGhostInteractionWithItem
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface|Interaction")
	bool GhostItemInteraction();
};
