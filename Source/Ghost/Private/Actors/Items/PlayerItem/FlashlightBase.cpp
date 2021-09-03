// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Items/PlayerItem/FlashlightBase.h"
#include "Components/SpotLightComponent.h"

AFlashlightBase::AFlashlightBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SpotLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLightComponent->SetupAttachment(GetStaticMeshComponent(), "Muzzle");
	SpotLightComponent->ToggleVisibility();
}

void AFlashlightBase::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AFlashlightBase::UseItemDirectly()
{
	SpotLightComponent->ToggleVisibility();

	if(GetOwner()->GetInstigatorController()) bItemUsed = SpotLightComponent->IsVisible();
	return true;
}

void AFlashlightBase::OnRep_ItemUsed()
{
	if(GetOwner()->GetInstigatorController()) return;

	UseItemDirectly();
}


