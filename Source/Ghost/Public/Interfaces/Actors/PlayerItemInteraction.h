// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Ghost/GhostCharacter.h"
#include "UObject/Interface.h"
#include "PlayerItemInteraction.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerItemInteraction : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GHOST_API IPlayerItemInteraction
{
	GENERATED_BODY()

	
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface")
	void PlayerInteractionWithItem(AGhostCharacter* Player);
};
