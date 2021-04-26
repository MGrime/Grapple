// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GrappleGameStateBase.generated.h"

/**
 * Stores stage data as this gets created each stage
 */
UCLASS()
class GRAPPLE_API AGrappleGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
protected:
	AGrappleGameStateBase();
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
public:

	// Times how long the player takes
	UPROPERTY(BlueprintReadWrite)
		float LevelTimer;
	
	// How many tokens have been collected
	UPROPERTY(BlueprintReadWrite)
		int32 LevelTokens;

	UPROPERTY(BlueprintReadWrite)
		bool bIsLevelDone;
	
};
