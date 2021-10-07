// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlayRoomGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GHOST_API APlayRoomGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void StartGhostActive();
};
