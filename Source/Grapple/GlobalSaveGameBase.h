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

	UPROPERTY()
	float Time;
};

USTRUCT()
struct FLevelTokenData
{
	GENERATED_BODY()
	
	UPROPERTY()
	int32 TokensCollected;

	UPROPERTY()
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
	
	UPROPERTY(EditAnywhere, Category = "Levels", SaveGame)
		TMap<int32,FLevelTimeData> LevelTimes;

	UPROPERTY(EditAnywhere, Category = "Levels", SaveGame)
		TMap<int32,FLevelTokenData> LevelTokens;


	// Blueprints don't inherently support the Maps above. So we combat this by exposing the needed detials through functions
	
	#pragma region BLUEPRINT FUNCTIONS

	UFUNCTION(BlueprintPure, Category = "Blueprint")
		bool HasLevelEntry(int32 levelIndex);

	// Returns best time for that level
	UFUNCTION(BlueprintPure, Category = "Blueprint")
		float GetLevelTime(int32 levelIndex);

	// Returns best tokens collected for that level
	UFUNCTION(BlueprintPure, Category = "Blueprint")
		int32 GetLevelTokensCollected(int32 levelIndex);

	// Returns max tokens for that level
	UFUNCTION(BlueprintPure, Category = "Blueprint")
		int32 GetLevelTokensMax(int32 levelIndex);
	
	#pragma endregion 
	
};
