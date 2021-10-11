// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ModesOfMovementPlayerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStaminaUseChanged, bool, bNewState);

USTRUCT(BlueprintType)
struct FMovementParam
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float MaxWalkSpeed;
	
	UPROPERTY(EditAnywhere)
	float MaxRunSpeed;
	
	UPROPERTY(EditAnywhere)
	float MaxSprintSpeed;

	/** usage loss per second */
	UPROPERTY(EditAnywhere)
	float DecreaseStaminaPreSec;

	/** stamina regeneration in second */
	UPROPERTY(EditAnywhere)
	float IncreaseStaminaPerSec;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GHOST_API UModesOfMovementPlayerComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	UFUNCTION()
	void OnRep_StaminaUse();

	UFUNCTION(Server, Unreliable)
	void Server_StartUseStamina();

	UFUNCTION(Server, Unreliable)
	void Server_StopUseStamina();

	UFUNCTION()
	void ReductionStamina();

	UFUNCTION()
	void StaminaRegeneration();

	void SetStaminaUse(bool NewState);

	void StaminaEnded();

public:	
	// Sets default values for this component's properties
	UModesOfMovementPlayerComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void OwnerStartUseStamina();
	void OwnerStopUseStamina();

	bool GetStaminaUse() const { return bStaminaUse; }
	FMovementParam GetMovementParam() const { return MovementParam; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	
	UPROPERTY(BlueprintAssignable)
	FStaminaUseChanged OnStaminaUseChanged;

private:

	UPROPERTY(ReplicatedUsing = OnRep_StaminaUse)
	bool bStaminaUse;
	
	UPROPERTY(EditAnywhere)
	float MaxStaminaValue;

	UPROPERTY(EditAnywhere)
	FMovementParam MovementParam;

	UPROPERTY(Replicated)
	float CurrentStaminaValue;

	/** The timer is responsible for the manipulation of stamina (reduction during use or regeneration) */
	FTimerHandle StaminaManipulationHandle;
};
