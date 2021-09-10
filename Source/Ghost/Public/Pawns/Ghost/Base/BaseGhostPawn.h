// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseGhostPawn.generated.h"

UCLASS(Abstract)
class GHOST_API ABaseGhostPawn : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseGhostPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
