// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ShootTargetActor.h"

#include "AIController.h"

#include "EnemyProjectileBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


EBTNodeResult::Type UBTTask_ShootTargetActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Need something to fire
	if (!ProjectileToFire)
	{
		return EBTNodeResult::Failed;
	}

	const auto BlackboardRef = OwnerComp.GetBlackboardComponent();
	if (!BlackboardRef)
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

	// Get target location and AI location
	const auto PlayerLocation = BlackboardRef->GetValueAsVector(GetSelectedBlackboardKey());
	const auto PawnLocation = AIOwner->GetActorLocation();

	// Get vector between points
	const auto AIToTarget = PlayerLocation - PawnLocation;

	// Get rotator from this vector
	const auto SpawnRotation = AIToTarget.Rotation();

	// Fire towards target
	GetWorld()->SpawnActor<AEnemyProjectileBase>(ProjectileToFire, PawnLocation, SpawnRotation);

	if (SoundToPlayOnFire)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			SoundToPlayOnFire,
			PawnLocation,
			SpawnRotation,
			0.2f
		);
	}

	return EBTNodeResult::Succeeded;
	
}
