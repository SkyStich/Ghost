// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseGhostPawn.generated.h"

class USkeletalMeshComponent;
class UCapsuleComponent;


UCLASS(Abstract)
class GHOST_API ABaseGhostPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseGhostPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* SkeletalMesh;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UPawnMovementComponent* PawnMovementComponent;
};
