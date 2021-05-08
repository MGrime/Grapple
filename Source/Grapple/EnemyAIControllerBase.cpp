// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIControllerBase.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


AEnemyAIControllerBase::AEnemyAIControllerBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
}


void AEnemyAIControllerBase::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerPawnRef = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (EnemyBehaviorTree)
	{
		RunBehaviorTree(EnemyBehaviorTree);
	}
}

bool AEnemyAIControllerBase::IsInFront(AActor* OtherActor) const
{
	const auto& AIPawn = GetPawn();

	// Copy required variables
	const auto AIForward = AIPawn->GetActorForwardVector();
	const auto OtherPosition = OtherActor->GetActorLocation();
	const auto AIPosition = AIPawn->GetActorLocation();

	// Calculate vector between
	auto AIToOther = OtherPosition - AIPosition;
	AIToOther.Normalize();

	// Calculate dot product
	const auto Dot = FVector::DotProduct(AIToOther, AIForward);

	// Return result
	return Dot > 0;


}

bool AEnemyAIControllerBase::IsInDistance(AActor* OtherActor) const
{
	return FVector::Dist(GetPawn()->GetActorLocation(), OtherActor->GetActorLocation()) < SightDistance;
}
