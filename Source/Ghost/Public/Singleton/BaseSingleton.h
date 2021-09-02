// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/StreamableManager.h"
#include "UObject/NoExportTypes.h"
#include "BaseSingleton.generated.h"

/**
 * 
 */
UCLASS()
class GHOST_API UBaseSingleton : public UObject
{
	GENERATED_BODY()

public:

	static UBaseSingleton& Get();
	FStreamableManager AssetLoader;
};
