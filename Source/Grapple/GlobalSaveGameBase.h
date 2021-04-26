// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GlobalSaveGameBase.generated.h"

/**
 * This is the save file for level data. each level stores best time and tokens collected
 */
USTRUCT()
struct FLevelTimeData
{
	GENERATED_BODY()
	
	float Time;
};

USTRUCT()
struct FLevelTokenData
{
	GENERATED_BODY()
	
	int32 TokensCollected;
	int32 TokensInLevel;
};

UCLASS()
class GRAPPLE_API UGlobalSaveGameBase : public USaveGame
{
	GENERATED_BODY()

public:
	UGlobalSaveGameBase();
	
	UPROPERTY(VisibleAnywhere, Category = "Config")
		FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = "Config")
		uint32 UserIndex;
	
	UPROPERTY(EditAnywhere, Category = "Levels")
		TMap<int32,FLevelTimeData> LevelTimes;

	UPROPERTY(EditAnywhere, Category = "Levels")
		TMap<int32,FLevelTokenData> LevelTokens;
	
};
