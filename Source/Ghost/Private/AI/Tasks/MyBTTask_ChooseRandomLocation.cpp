// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/MyBTTask_ChooseRandomLocation.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "AIControllers/BaseAIController/BaseGhostAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlackboardNamespace/BlackboardKeys.h"

UMyBTTask_ChooseRandomLocation::UMyBTTask_ChooseRandomLocation()
{
	FindRadius = 280.f;
}

EBTNodeResult::Type UMyBTTask_ChooseRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	/** init base var */
	FVector TargetLocation = FVector::ZeroVector;
	FVector const Origin =	OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
	FNavLocation Loc = FNavLocation();
	
	/** set current navigation system */
	UNavigationSystemV1* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	if(NavSystem->GetRandomPointInNavigableRadius(Origin, FindRadius, Loc, nullptr))
	{
		OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsVector(bb_keys::Target_Location, Loc.Location);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;
}
