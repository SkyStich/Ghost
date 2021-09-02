// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Items/Base/PlayerItemBase.h"
#include "Engine/DataAsset.h"
#include "PlayerItemDataAsset.generated.h"

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FAsyncSpawnItem, bool, FSynthBenchmarkResults, FSoftObjectPath, Ref, APlayerItemBase*, Item);

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemId;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bConsumable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TAssetSubclassOf<APlayerItemBase> ItemClass;
};

DECLARE_LOG_CATEGORY_EXTERN(LogPlayerDataAsset, Log, All);

UCLASS(Blueprintable)
class GHOST_API UPlayerItemDataAsset : public UDataAsset
{
	GENERATED_BODY()

	static void OnAsyncSpawnActorComplete(UObject* WorldContext, FSoftObjectPath Ref, FTransform Transform, FAsyncSpawnItem CallBack);

public:

	/** Spawn async actor */
	UFUNCTION(BlueprintCallable, Category = "DataAsset", meta=(DisplayName = "Spawn player item (async)"))
	static bool AsyncSpawnActor(UObject* WorldContext, TAssetSubclassOf<APlayerItemBase> Class, const FTransform& Transform, const FAsyncSpawnItem& CallBack);

private:

	UPROPERTY(EditAnyWhere)
	TMap<FName, FItemData> ItemData;
};
