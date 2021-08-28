
#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Interfaces/Actors/PlayerItemInteraction.h"
#include "BaseItem.generated.h"

class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemRemoveFromStorageComplete, class ABaseItem*, Item);

/** Base item with which to interact */
UCLASS(Abstract)
class GHOST_API ABaseItem : public AStaticMeshActor, public IPlayerItemInteraction
{
	GENERATED_BODY()

	UFUNCTION()
	void OnRep_IsStorage();
	
public:	
	
	ABaseItem();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PlayerInteractionWithItem_Implementation(AGhostCharacter* Player) override;

	/** called when an object is removed from the owner's inventory */
	void RemoveFromStorage();

	/** called from the server and connected clients when adding an object to the player's inventory */
	void AddToStorage();

protected:
	
	virtual void BeginPlay() override;

public:

	FItemRemoveFromStorageComplete OnItemRemoveFromStorageComplete;
private:

	/** true if actor in storage */
	UPROPERTY(ReplicatedUsing = OnRep_IsStorage)
	bool IsInStorage;
};
