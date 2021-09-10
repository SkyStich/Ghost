// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_MoveToTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlackboardNamespace/BlackboardKeys.h"

UBTTask_MoveToTarget::UBTTask_MoveToTarget()
{
	NodeName = TEXT("MoveToTarget");
}

EBTNodeResult::Type UBTTask_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	/** Get current target */
	auto const TargetActor = OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsObject(bb_keys::Target_Actor);

	if(TargetActor)
	{
		/** move to target location */
		auto const Location = Cast<AActor>(TargetActor)->GetActorLocation();
		OwnerComp.GetAIOwner()->MoveToLocation(Location, 30.f);

		/** return with succeeded */
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	/** return with failed */
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;
}
