// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Ghost/Base/BaseGhostPawn.h"
#include "AIControllers/BaseAIController/BaseGhostAIController.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

ABaseGhostPawn::ABaseGhostPawn()
{
	PrimaryActorTick.bCanEverTick = false;
	
	bReplicates = true;

	/** Set default AI controller */
	AIControllerClass = ABaseGhostAIController::StaticClass();

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMesh->SetupAttachment(RootComponent);

	PawnMovementComponent = CreateDefaultSubobject<UPawnMovementComponent>(TEXT("MovementComponent"));
}

void ABaseGhostPawn::BeginPlay()
{
	Super::BeginPlay();
	
}