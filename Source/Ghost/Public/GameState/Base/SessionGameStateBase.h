// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SessionGameStateBase.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FPlayerInHouseChanged, bool, bState);

UCLASS()
class GHOST_API ASessionGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:

	ASessionGameStateBase();

	void SetPlayersInHouse(bool bState) { bPlayersInHouse = bState; }
	
	FPlayerInHouseChanged GetOnPlayerInHouseChanged() const { return OnPlayerInHouseChanged; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetPlayersInHouse() const { return bPlayersInHouse; }

private:

	/** only on server. True if at least one player in house */
	bool bPlayersInHouse;

	UPROPERTY()
	FPlayerInHouseChanged OnPlayerInHouseChanged;
};
