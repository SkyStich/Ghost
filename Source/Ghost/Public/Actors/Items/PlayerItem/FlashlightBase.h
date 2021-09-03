// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Items/Base/PlayerItemBase.h"
#include "FlashlightBase.generated.h"

class USpotLightComponent;

UCLASS(Abstract)
class GHOST_API AFlashlightBase : public APlayerItemBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlashlightBase();

	virtual bool UseItemDirectly() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnRep_ItemUsed() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USpotLightComponent* SpotLightComponent;
};
