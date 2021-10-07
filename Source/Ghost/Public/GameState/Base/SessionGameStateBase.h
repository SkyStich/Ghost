// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SessionGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class GHOST_API ASessionGameStateBase : public AGameState
{
	GENERATED_BODY()

public:

	ASessionGameStateBase();

	void SetPlayersInHouse(bool bState) { bPlayersInHouse = bState; }

	UFUNCTION(BlueprintPure)
	bool GetPlayersInHouse() const { return bPlayersInHouse; }

private:

	/** only on server */
	bool bPlayersInHouse;
};
