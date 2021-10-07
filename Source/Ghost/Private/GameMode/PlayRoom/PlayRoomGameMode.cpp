// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PlayRoom/PlayRoomGameMode.h"

#include "GameState/Base/SessionGameStateBase.h"

void APlayRoomGameMode::StartGhostActive()
{
	GameStateClass = ASessionGameStateBase::StaticClass();
}
