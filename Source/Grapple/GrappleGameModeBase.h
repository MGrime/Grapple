// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SettingsSaveGame.h"
#include "Sound/SoundMix.h"

#include "GrappleGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class GRAPPLE_API AGrappleGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
		virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
		void LoadLevel(FName LevelName);

	UFUNCTION(BlueprintCallable)
		void SaveSettings();
	
	#pragma region VARIABLE ACCESS

	UFUNCTION(BlueprintCallable)
		USettingsSaveGame* GetSaveGame();

	#pragma endregion

	#pragma region ASYNC CALLBACKS

	UFUNCTION()
		void SettingsSaveComplete(const FString& SlotName, const int32 UserIndex, bool bSuccess);
	
	#pragma endregion
		

private:

	#pragma region SOUND
	
	#pragma endregion
	
	#pragma region SAVE GAMES
	
	UPROPERTY(VisibleAnywhere, Category = "Save Game")
	USettingsSaveGame* SettingsSaveGame;	

	#pragma endregion
	
	
};
