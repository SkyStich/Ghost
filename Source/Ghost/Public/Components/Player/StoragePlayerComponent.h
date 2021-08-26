// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Items/Base/BaseItem.h"
#include "Components/ActorComponent.h"
#include "StoragePlayerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNewCurrentItem, ABaseItem*, Item);

/*
 * the object is embedded in the player to manage inventory
 */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GHOST_API UStoragePlayerComponent : public UActorComponent
{
	GENERATED_BODY()

	UFUNCTION()
	void OnRep_CurrentItem() const;

	void SetCurrentItem(ABaseItem* const ItemToSet);

public:	

	UStoragePlayerComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FORCEINLINE TArray<ABaseItem*> GetPlayerItems() const { return Items; }
	FORCEINLINE ABaseItem* GetPlayerCurrentItem() const { return CurrentItem; }

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	bool AddItemToStorage(ABaseItem* const ItemToAdd);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	bool RemoveItemFromStorage(ABaseItem* const ItemToRemove);

	void ShowItem(ABaseItem* Item);
	void HiddenItem(ABaseItem* Item);
	
protected:

	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FNewCurrentItem OnNewCurrentItem;

private:

	UPROPERTY(Replicated)
	TArray<ABaseItem*> Items;
	
	UPROPERTY(ReplicatedUsing = OnRep_CurrentItem)
	ABaseItem* CurrentItem;
};
