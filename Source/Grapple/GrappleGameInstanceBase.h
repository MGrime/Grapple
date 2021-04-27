// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GlobalSaveGameBase.h"
#include "SettingsSaveGame.h"
#include "Engine/GameInstance.h"
#include "GrappleGameInstanceBase.generated.h"

/**
 * 
 */
UCLASS()
class GRAPPLE_API UGrappleGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()
protected:
	virtual void Init() override;
public:
	#pragma region VARIABLES
	
	UPROPERTY(BlueprintReadWrite)
		bool bIsMainMenu = true;

	UPROPERTY(BlueprintReadWrite)
		bool bIsLevelSelect = false;

	UPROPERTY(BlueprintReadWrite)
		UGlobalSaveGameBase* ActiveSaveGame = nullptr;

	UPROPERTY(BlueprintReadWrite)
		USettingsSaveGame* SettingsSaveGame = nullptr;

	#pragma endregion

	#pragma region FUNCTIONS

	UFUNCTION(BlueprintCallable)
		void SaveSettings();

	// Saves active save data.
	UFUNCTION(BlueprintCallable)
		void SaveActive();

	#pragma endregion

	#pragma region ASYNC CALLBACKS

	UFUNCTION()
		void SettingsSaveComplete(const FString& SlotName, const int32 UserIndex, bool bSuccess);

	UFUNCTION()
		void ActiveSaveComplete(const FString& SlotName, const int32 UserIndex, bool bSuccess);

	#pragma endregion
	
};
