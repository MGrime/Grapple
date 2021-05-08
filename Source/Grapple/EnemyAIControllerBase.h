// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class GRAPPLE_API AEnemyAIControllerBase : public AAIController
{
	GENERATED_BODY()
public:

	AEnemyAIControllerBase();
	
	UFUNCTION(BlueprintPure)
		bool IsInFront(AActor* OtherActor) const;

	UFUNCTION(BlueprintPure)
		bool IsInDistance(AActor* OtherActor) const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		UBehaviorTree* EnemyBehaviorTree;

	UPROPERTY(EditAnywhere)
		float SightDistance = 5000.0f;

private:
	UPROPERTY()
		APawn* PlayerPawnRef;
};
