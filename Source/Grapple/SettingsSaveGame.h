// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SettingsSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class GRAPPLE_API USettingsSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	USettingsSaveGame();

	#pragma region CONFIG VARS

	UPROPERTY(VisibleAnywhere, Category = "Config")
		FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = "Config")
		uint32 UserIndex;

	#pragma endregion

	#pragma region SAVE VALUES

	UPROPERTY(EditAnywhere)
		float MusicSoundVolume = 1.0f;
	
	#pragma endregion

	#pragma region FUNCTIONS
	
	UFUNCTION(BlueprintPure)
		float GetMusicVolumeBP();

	UFUNCTION(BlueprintCallable)
		void SetMusicVolumeBP(float Value);

	#pragma endregion
};
