// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllers/BaseAIController/BaseGhostAIController.h"
#include "AI/NavigationSystemBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

ABaseGhostAIController::ABaseGhostAIController()
{
	PrimaryActorTick.bCanEverTick = false;

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	
	SetupPerceptionSystem();
}

void ABaseGhostAIController::SetupPerceptionSystem()
{
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent"));

	SightConfig->SightRadius = SightRadius;

	/** maximum range for visibility of a target that has already been spotted (sight radius + 20%) */
	SightConfig->LoseSightRadius = SightRadius * 1.2;

	/** installing peripheral vision */
	SightConfig->PeripheralVisionAngleDegrees = 80.f;

	SightConfig->SetMaxAge(3.f);

	SightConfig->AutoSuccessRangeFromLastSeenLocation = 5.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseGhostAIController::OnSightUpdate);
}

void ABaseGhostAIController::OnSightUpdate(AActor* SeeActor, FAIStimulus const Stimulus)
{
	if(Stimulus.WasSuccessfullySensed())
	{
		/** if target be detected */
	}
	else
	{
		
	}
}

