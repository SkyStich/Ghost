// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Triggers/BorderOfRoom/BorderOfRoomTriggerVolume.h"
#include "Pawns/Ghost/Base/BaseGhostPawn.h"
#include "BaseTestGhost.generated.h"

/**
 * 
 */
UCLASS()
class GHOST_API ABaseTestGhost : public ABaseGhostPawn
{
	GENERATED_BODY()
	
	ABaseTestGhost();

public:

	UFUNCTION(BlueprintCallable)
	void IncreaseLevelAggressive(int32 const Value) { LevelAggressive += Value; }
	
	UFUNCTION(BlueprintCallable)
	void DecreaseLevelAggressive(int32 const Value) { LevelAggressive -= Value; }
	
	UFUNCTION(BlueprintCallable)
	void StandAtWaitingPoint();
  
private:

	UPROPERTY()
	int32 LevelAggressive;
};
