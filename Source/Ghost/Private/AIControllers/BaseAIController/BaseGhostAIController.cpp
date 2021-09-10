// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllers/BaseAIController/BaseGhostAIController.h"
#include "AI/NavigationSystemBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BlackboardNamespace/BlackboardKeys.h"
#include "Kismet/KismetMathLibrary.h"

ABaseGhostAIController::ABaseGhostAIController()
{
	PrimaryActorTick.bCanEverTick = false;

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	Blackboard->InitializeBlackboard(*BlackboardData);
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
		auto const CurrentTarget = GetBlackboardComponent()->GetValueAsObject(bb_keys::Target_Actor);

		if(CurrentTarget)
		{
			UpdateTargetActor(SeeActor, CurrentTarget);
		}
		else
		{
			Blackboard->SetValueAsObject(bb_keys::Target_Actor, SeeActor);
		}
	}
	else
	{
		if(SeeActor == Blackboard->GetValueAsObject(bb_keys::Target_Actor))
		{
			Blackboard->SetValueAsObject(bb_keys::Target_Actor, nullptr);
		}
	}
}

void ABaseGhostAIController::UpdateTargetActor(AActor* SeeActor, UObject* CurrentTarget)
{
	float const CurrentTargetDistance = UKismetMathLibrary::Vector_Distance(GetPawn()->GetActorLocation(), Cast<AActor>(CurrentTarget)->GetActorLocation());
	float const NewTargetDistance = UKismetMathLibrary::Vector_Distance(GetPawn()->GetActorLocation(), SeeActor->GetActorLocation());

	if(CurrentTargetDistance > NewTargetDistance)
		Blackboard->SetValueAsObject(bb_keys::Target_Actor, SeeActor);
}

