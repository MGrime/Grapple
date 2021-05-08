// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChoosePatrolLocation.h"

#include "EnemyAIControllerBase.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_ChoosePatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Check we are attached to an enemy AI
	const auto AIOwner = Cast<AEnemyAIControllerBase>(OwnerComp.GetAIOwner());
	if (!AIOwner)
	{
		return EBTNodeResult::Failed;
	}

	// Get the controlled pawn
	const auto AIPawn = AIOwner->GetPawn();
	FNavLocation RandomLocation;

	// Access the worlds navigation system
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	NavSys->GetRandomReachablePointInRadius(AIPawn->GetActorLocation(), RandomRadius, RandomLocation);

	// Set selected value
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), RandomLocation);
	return EBTNodeResult::Succeeded;
	
}
