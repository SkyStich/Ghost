// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

#include "GhostCharacter.generated.h"

class UStoragePlayerComponent;
class UCameraComponent;
class ABaseItem;

UCLASS(config=Game)
class AGhostCharacter : public ACharacter
{
	GENERATED_BODY()

	UFUNCTION()
	void OnNewCurrentWeaponEvent(ABaseItem* NewItem);

	/**called when an object is removed from the owner's inventory. On server*/
	UFUNCTION()
	void OnItemRemoveEvent(ABaseItem* NewItem);

	UFUNCTION()
	void PressedInteraction();
	
	UFUNCTION(Server, Unreliable)
	void Server_InteractionWithItem();
	
	UFUNCTION()
    void UseItemDirectlyPressed();

	UFUNCTION()
    void DoorInteractionPressed();

	UFUNCTION()
	void DropInteractionReleased();

	UFUNCTION(Server, Unreliable)
	void Server_UpdateDoorRotate(float Rate);

	UFUNCTION(Server, Unreliable)
	void Server_DoorInteractionTrace();
	
	UFUNCTION(Server, Unreliable)
	void Server_FinishDoorInteraction();

	AActor* DropLineTraceFromInteraction();
	void UpdateDoorRotate(float Rate);
	bool DropInteractionDoorTrace(FHitResult& OutHit);

protected:

	virtual void BeginPlay() override;
	virtual void AddControllerYawInput(float Val) override;

public:
	AGhostCharacter();

	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE UStoragePlayerComponent* GetStoragePlayerComponent() const { return StoragePlayerComponent; }

protected:

	void MoveForward(float Value);
	void MoveRight(float Value);

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
public:
	
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
    float BaseTurnRate;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
    float BaseLookUpRate;
    
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StorageComponent")
	UStoragePlayerComponent* StoragePlayerComponent;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "StorageComponent")
	UAIPerceptionStimuliSourceComponent* StimuliSourceComponent;

private:

	UPROPERTY(EditAnywhere)
	float DoorTurnRate;

	UPROPERTY()
	UPrimitiveComponent* InteractionDoor;
};

