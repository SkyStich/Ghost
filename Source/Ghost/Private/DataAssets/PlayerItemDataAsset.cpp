// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/PlayerItemDataAsset.h"
#include "Engine/StreamableManager.h"
#include "Singleton/BaseSingleton.h"

DEFINE_LOG_CATEGORY(LogPlayerDataAsset);

bool UPlayerItemDataAsset::AsyncSpawnActor(UObject* WorldContext, TAssetSubclassOf<APlayerItemBase> Class, const FTransform& Transform, const FAsyncSpawnItem& CallBack)
{
	if(Class.IsNull())
	{
		FString const InstigatorName = WorldContext ? *WorldContext->GetFullName() : TEXT("Uncknown");
		UE_LOG(LogPlayerDataAsset, Error, TEXT("UPlayerItemDataAsset::AsyncSpawnActor --Asset class is NULL"), *InstigatorName);
		return false;
	}

	/** Load asset in memory */
	FStreamableManager& AssetLoader = UBaseSingleton::Get().AssetLoader;
	FSoftObjectPath const Ref = Class.ToSoftObjectPath();
	AssetLoader.RequestAsyncLoad(Ref, FStreamableDelegate::CreateStatic(&UPlayerItemDataAsset::OnAsyncSpawnActorComplete, WorldContext, Ref, Transform, CallBack));
	return true;
}

void UPlayerItemDataAsset::OnAsyncSpawnActorComplete(UObject* WorldContext, FSoftObjectPath Ref, FTransform Transform, FAsyncSpawnItem CallBack)
{
	APlayerItemBase* SpawnActor = nullptr;

	/** Asset in memory. Load class of object */
	UClass* ActorClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *Ref.ToString()));
	if(ActorClass)
	{
		/** Spawn actor */
		SpawnActor = WorldContext->GetWorld()->SpawnActor<APlayerItemBase>(ActorClass, Transform);
	}
	else
	{
		UE_LOG(LogPlayerDataAsset, Error, TEXT("UPlayerItemDataAsset::OnAsyncSpawnActorComplete --Create Actor class complete with fail"),
			*WorldContext->GetFullName());
	}

	/** execute delegate */
	CallBack.Execute(SpawnActor != nullptr, Ref, SpawnActor);
}
