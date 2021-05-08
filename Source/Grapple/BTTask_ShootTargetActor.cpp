// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ShootTargetActor.h"

#include "AIController.h"

#include "EnemyProjectileBase.h"


EBTNodeResult::Type UBTTask_ShootTargetActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Need something to fire
	if (!ProjectileToFire)
	{
		return EBTNodeResult::Failed;
	}

	// We have something to fire, so we want to fire it
	const auto AIOwner = OwnerComp.GetAIOwner()->GetPawn();
	// This very very likely wont happen but its a worthy check
	if (!AIOwner)
	{
		return EBTNodeResult::Failed;
	}

	// Spawn projectile
	const auto SpawnLocation = AIOwner->GetActorLocation();
	const auto SpawnRotation = AIOwner->GetActorRotation();
	GetWorld()->SpawnActor<AEnemyProjectileBase>(ProjectileToFire, SpawnLocation, SpawnRotation);

	return EBTNodeResult::Succeeded;
	
}
