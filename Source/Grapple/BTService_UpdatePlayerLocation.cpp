// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdatePlayerLocation.h"

#include "EnemyAIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void UBTService_UpdatePlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const auto EnemyAIController = Cast<AEnemyAIControllerBase>(OwnerComp.GetAIOwner());

	// This means we are instanced
	if (EnemyAIController)
	{
		// If no player ref
		if (!PlayerRef)
		{
			UpdatePlayerRef();
		}
		else
		{
			// Check if valid
			if (!IsValid(PlayerRef))
			{
				UpdatePlayerRef();
			}
		}

		if (EnemyAIController->LineOfSightTo(PlayerRef) && EnemyAIController->IsInFront(PlayerRef) && EnemyAIController->IsInDistance(PlayerRef))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerRef->GetActorLocation());
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
		}
	}

	
}

void UBTService_UpdatePlayerLocation::UpdatePlayerRef()
{
	PlayerRef = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}
