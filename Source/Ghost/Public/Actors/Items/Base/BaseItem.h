
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"

class UStaticMeshComponent;

/** Base item with which to interact */
UCLASS(Abstract)
class GHOST_API ABaseItem : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ABaseItem();

	FORCEINLINE UStaticMeshComponent* GetItemMesh() const { return StaticMesh; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;
};
