// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

	UFUNCTION()
	void PressedInteraction();
	
	UFUNCTION(Server, Unreliable)
	void Server_InteractionWithItem();

	AActor* DropLineTraceFromInteraction();

protected:

	virtual void BeginPlay() override;

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

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StorageComponent")
	UStoragePlayerComponent* StoragePlayerComponent;
};

