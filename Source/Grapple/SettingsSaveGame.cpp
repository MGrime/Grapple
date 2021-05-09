// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsSaveGame.h"

#include "GrappleGameInstanceBase.h"
#include "Kismet/GameplayStatics.h"

USettingsSaveGame::USettingsSaveGame()
{
	SaveSlotName = TEXT("SettingsSlot");

	UserIndex = 0;
	
}

float USettingsSaveGame::GetMusicVolumeBP()
{
	return MusicSoundVolume;
}

void USettingsSaveGame::SetMusicVolumeBP(float Value)
{
	MusicSoundVolume = Value;

	// Check to update current sound
	const auto GameInstance = Cast<UGrappleGameInstanceBase>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GameInstance)
	{
		GameInstance->UpdateMusicVolume();
	}
}
