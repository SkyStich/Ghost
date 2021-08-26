// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Items/Base/BaseItem.h"
#include "Interfaces/Actors/GhostInteractionWithItem.h"
#include "DecorativeItemBase.generated.h"

/**
 * Base item for only interaction of the player and the ghost
 */
UCLASS(Abstract)
class GHOST_API ADecorativeItemBase : public ABaseItem, public IGhostInteractionWithItem
{
	GENERATED_BODY()

public:

	ADecorativeItemBase();
};
