// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SettingsSaveGame.h"
#include "Sound/SoundMix.h"

#include "GrappleGameModeBase.generated.h"

class ALevelDetailsActor;
/**
 * This is created every single level load
 */
UCLASS()
class GRAPPLE_API AGrappleGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
private:
	virtual void BeginPlay() override;
public:
	
	UFUNCTION(BlueprintCallable)
		void LoadLevel(FName LevelName);

	// Loads level clearing any instance UI state
	UFUNCTION(Exec)
		void LoadLevelCon(FName LevelName);

	UFUNCTION(BlueprintCallable)
		FString GetActiveLevelName();

	UFUNCTION(BlueprintCallable)
		float GetActiveLevelGoldTime();

private:
	UPROPERTY(VisibleAnywhere)
		ALevelDetailsActor* ActiveLevelDetails;

};
