// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "TransitionToHouseTrigger.generated.h"

/**
 * 
 */
UCLASS()
class GHOST_API ATransitionToHouseTrigger : public ATriggerBox
{
	GENERATED_BODY()

public:

	ATransitionToHouseTrigger();

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
    void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	UFUNCTION()
    void OnComponentEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
