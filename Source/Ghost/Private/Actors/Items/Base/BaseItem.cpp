// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Items/Base/BaseItem.h"
#include "Components/StaticMeshComponent.h"
// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	NetUpdateFrequency = 10.f;
	NetCullDistanceSquared = 5000.f;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StatiMesh"));
	StaticMesh->SetupAttachment(RootComponent);
}

void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	
}

