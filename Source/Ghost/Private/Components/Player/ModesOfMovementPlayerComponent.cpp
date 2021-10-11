// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Player/ModesOfMovementPlayerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

UModesOfMovementPlayerComponent::UModesOfMovementPlayerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicated(true);

#if WITH_EDITOR
	PrimaryComponentTick.bCanEverTick = true;
#endif
}

// Called when the game starts
void UModesOfMovementPlayerComponent::BeginPlay()
{
	Super::BeginPlay();	
	
	if(GetOwnerRole() == ROLE_Authority)
	{
		CurrentStaminaValue = MaxStaminaValue;
	}
}

void UModesOfMovementPlayerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("CurrentStamina: %f"), CurrentStaminaValue), true, false, FColor::Blue, 0.f);
}

void UModesOfMovementPlayerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UModesOfMovementPlayerComponent, bStaminaUse);
	DOREPLIFETIME_CONDITION(UModesOfMovementPlayerComponent, CurrentStaminaValue, COND_OwnerOnly);
}

void UModesOfMovementPlayerComponent::OnRep_StaminaUse()
{
	OnStaminaUseChanged.Broadcast(bStaminaUse);
}

void UModesOfMovementPlayerComponent::SetStaminaUse(bool NewState)
{
	bStaminaUse = NewState;
	OnRep_StaminaUse();
}

void UModesOfMovementPlayerComponent::OwnerStartUseStamina()
{
	if(!bStaminaUse && CurrentStaminaValue > 0)
	{		
		SetStaminaUse(true);
		Server_StartUseStamina();
	}
}

void UModesOfMovementPlayerComponent::OwnerStopUseStamina()
{
	if(bStaminaUse)
	{
		SetStaminaUse(false);
		Server_StopUseStamina();
	}
}

void UModesOfMovementPlayerComponent::Server_StartUseStamina_Implementation()
{
	if(!bStaminaUse && CurrentStaminaValue > 0)
	{
		SetStaminaUse(true);

		CurrentStaminaValue -= MovementParam.DecreaseStaminaPreSec * 0.2;

		GetWorld()->GetTimerManager().ClearTimer(StaminaManipulationHandle);
		GetWorld()->GetTimerManager().SetTimer(StaminaManipulationHandle,this, &UModesOfMovementPlayerComponent::ReductionStamina, 0.2f, true);
	}
}

void UModesOfMovementPlayerComponent::Server_StopUseStamina_Implementation()
{
	if(bStaminaUse)
	{
		SetStaminaUse(false);
		
		GetWorld()->GetTimerManager().ClearTimer(StaminaManipulationHandle);
		GetWorld()->GetTimerManager().SetTimer(StaminaManipulationHandle,this, &UModesOfMovementPlayerComponent::StaminaRegeneration, 0.2f, true, 4);
	}
}

void UModesOfMovementPlayerComponent::ReductionStamina()
{
	if(CurrentStaminaValue <= 0)
	{
		StaminaEnded();
		return;
	}
	CurrentStaminaValue -= MovementParam.DecreaseStaminaPreSec * 0.2f;
}

void UModesOfMovementPlayerComponent::StaminaRegeneration()
{
	if(CurrentStaminaValue >= MaxStaminaValue)
	{
		CurrentStaminaValue = MaxStaminaValue;
		GetWorld()->GetTimerManager().ClearTimer(StaminaManipulationHandle);
		return;
	}
	CurrentStaminaValue += MovementParam.IncreaseStaminaPerSec * 0.2;
}

void UModesOfMovementPlayerComponent::StaminaEnded()
{
	CurrentStaminaValue = 0.f;

	SetStaminaUse(false);
	GetWorld()->GetTimerManager().ClearTimer(StaminaManipulationHandle);
	GetWorld()->GetTimerManager().SetTimer(StaminaManipulationHandle,this, &UModesOfMovementPlayerComponent::StaminaRegeneration, 0.2f, true, 4);
}