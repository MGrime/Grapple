// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelDetailsActor.generated.h"

/*
 * Stores information in the level. Only one per level
 */

UCLASS()
class GRAPPLE_API ALevelDetailsActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelDetailsActor();

	UPROPERTY(EditAnywhere)
		FString LevelName;

	// This is the longest time for gold. Silver adds 1/3, bronze adds another 1/3
	UPROPERTY(EditAnywhere)
		float TimeForGold;
};
