
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Actors/PlayerItemInteraction.h"
#include "BaseItem.generated.h"

class UStaticMeshComponent;

/** Base item with which to interact */
UCLASS(Abstract)
class GHOST_API ABaseItem : public AActor, public IPlayerItemInteraction
{
	GENERATED_BODY()

	UFUNCTION()
	void OnRep_IsStorage();
	
public:	
	
	ABaseItem();

	FORCEINLINE UStaticMeshComponent* GetItemMesh() const { return StaticMesh; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PlayerInteractionWithItem_Implementation(AGhostCharacter* Player) override;

protected:
	
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(ReplicatedUsing = OnRep_IsStorage)
	bool IsInStorage;
};
