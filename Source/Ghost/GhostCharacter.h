// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

#include "GhostCharacter.generated.h"

class UStoragePlayerComponent;
class UCameraComponent;
class ABaseItem;
class UModesOfMovementPlayerComponent;

UCLASS(config=Game)
class AGhostCharacter : public ACharacter
{
	GENERATED_BODY()

	UFUNCTION()
	void OnUseStaminaEvent(bool bState);

	UFUNCTION()
	void OnNewCurrentWeaponEvent(ABaseItem* NewItem);

	UFUNCTION()
	void OnRep_PlayerTurn();

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


public:
	AGhostCharacter();

	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE UStoragePlayerComponent* GetStoragePlayerComponent() const { return StoragePlayerComponent; }
	FORCEINLINE UModesOfMovementPlayerComponent* GetModesOfMovementComponent() const { return ModesOfMovementComponent; }

protected:

	void MoveForward(float Value);
	void MoveRight(float Value);

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	
	virtual void BeginPlay() override;
	virtual void AddControllerYawInput(float Val) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
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

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ModesOfMovementComponent")
	UModesOfMovementPlayerComponent* ModesOfMovementComponent;

private:

	UPROPERTY(EditAnywhere)
	float DoorTurnRate;

	UPROPERTY(Replicated)
	UPrimitiveComponent* InteractionDoor;

	UPROPERTY(ReplicatedUsing=OnRep_PlayerTurn)
	float PlayerTurn;
	
	bool bMoveForward;
};