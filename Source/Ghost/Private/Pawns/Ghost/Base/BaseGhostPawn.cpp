// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Ghost/Base/BaseGhostPawn.h"
#include "AIControllers/BaseAIController/BaseGhostAIController.h"

ABaseGhostPawn::ABaseGhostPawn()
{
	PrimaryActorTick.bCanEverTick = false;
	
	bReplicates = true;

	AIControllerClass = ABaseGhostAIController::StaticClass();
}

void ABaseGhostPawn::BeginPlay()
{
	Super::BeginPlay();
	
}