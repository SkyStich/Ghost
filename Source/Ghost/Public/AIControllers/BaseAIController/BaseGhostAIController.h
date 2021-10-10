// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"

#include "BaseGhostAIController.generated.h"

class UBehaviorTreeComponent;
class UAISenseConfig_Sight;

UCLASS(Abstract)
class GHOST_API ABaseGhostAIController : public AAIController
{
	GENERATED_BODY()

private:

	void SetupPerceptionSystem();
	void UpdateTargetActor(AActor* SeeActor, UObject* CurrentTarget);

protected:

	UFUNCTION()
	void OnSightUpdate(AActor* SeeActor, FAIStimulus const Stimulus);
	
	virtual void BeginPlay() override;

public:

	ABaseGhostAIController();

protected:

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	UBehaviorTree* PhysicalFormTree;
	
	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	UBehaviorTree* GhostFormTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAISenseConfig_Sight* SightConfig;

private:

	UPROPERTY(EditAnywhere)
	float SightRadius;

	UPROPERTY(EditAnywhere)
	UBlackboardData* BlackboardData;
};
