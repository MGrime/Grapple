// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleGameInstanceBase.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void UGrappleGameInstanceBase::Init()
{
	Super::Init();
	// by default attempt to load from slot 1, if fail create a blank save. Can override later on
	ActiveSaveGame = Cast<UGlobalSaveGameBase>(UGameplayStatics::LoadGameFromSlot(TEXT("SaveSlotOne"), 0));
	if (!ActiveSaveGame)
	{
		// Nothing loaded so create blank
		ActiveSaveGame = Cast<UGlobalSaveGameBase>(UGameplayStatics::CreateSaveGameObject(UGlobalSaveGameBase::StaticClass()));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Loaded save game from disk!"));
	}

	// Same for settings
	SettingsSaveGame = Cast<USettingsSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("SettingsSlot"), 0));
	if (!SettingsSaveGame)
	{
		SettingsSaveGame = Cast<USettingsSaveGame>(UGameplayStatics::CreateSaveGameObject(USettingsSaveGame::StaticClass()));
	}
	
}

void UGrappleGameInstanceBase::SaveSettings()
{
	// Bind callback
	FAsyncSaveGameToSlotDelegate SavedDelegate;
	SavedDelegate.BindUObject(this, &UGrappleGameInstanceBase::SettingsSaveComplete);

	// Async save
	UGameplayStatics::AsyncSaveGameToSlot(SettingsSaveGame, SettingsSaveGame->SaveSlotName, SettingsSaveGame->UserIndex, SavedDelegate);
}

void UGrappleGameInstanceBase::SaveActive()
{
	// Bind callback
	FAsyncSaveGameToSlotDelegate SavedDelegate;
	SavedDelegate.BindUObject(this, &UGrappleGameInstanceBase::ActiveSaveComplete);

	// Async save
	UGameplayStatics::AsyncSaveGameToSlot(ActiveSaveGame, ActiveSaveGame->SaveSlotName, ActiveSaveGame->UserIndex, SavedDelegate);
}

void UGrappleGameInstanceBase::PlayTitleMusic()
{
	if (MainMenuMusic)
	{
		if (!MainMenuMusicInstance)
		{
			MainMenuMusicInstance = UGameplayStatics::SpawnSound2D(
				GetWorld(),
				MainMenuMusic,
				SettingsSaveGame->MusicSoundVolume,
				1.0f,
				0.0f,
				nullptr,
				true,
				false
			);
		}
		else
		{
			MainMenuMusicInstance->Play(0.0f);
		}
	}
	
	
}

void UGrappleGameInstanceBase::ToggleTitleMusic()
{
	if (MainMenuMusicInstance->IsPlaying())
	{
		MainMenuMusicInstance->SetPaused(!MainMenuMusicInstance->bIsPaused);
	}
}

void UGrappleGameInstanceBase::StopTitleMusic()
{
	if (MainMenuMusicInstance->IsPlaying())
	{
		MainMenuMusicInstance->Stop();
	}
}

void UGrappleGameInstanceBase::PlayLevelMusic()
{
	if (LevelMusic)
	{
		if (!LevelMusicInstance)
		{
			LevelMusicInstance = UGameplayStatics::SpawnSound2D(
				GetWorld(),
				LevelMusic,
				SettingsSaveGame->MusicSoundVolume,
				1.0f,
				0.0f,
				nullptr,
				true,
				false
			);
		}
		else
		{
			LevelMusicInstance->Play(0.0f);
		}
	}


}

void UGrappleGameInstanceBase::ToggleLevelMusic()
{
	if (LevelMusicInstance->IsPlaying())
	{
		LevelMusicInstance->SetPaused(!LevelMusicInstance->bIsPaused);
	}
}

void UGrappleGameInstanceBase::StopLevelMusic()
{
	if (LevelMusicInstance->IsPlaying())
	{
		LevelMusicInstance->Stop();
	}
}

void UGrappleGameInstanceBase::UpdateMusicVolume()
{
	MainMenuMusicInstance->SetVolumeMultiplier(SettingsSaveGame->MusicSoundVolume);
	LevelMusicInstance->SetVolumeMultiplier(SettingsSaveGame->MusicSoundVolume);
}

void UGrappleGameInstanceBase::ActiveSaveComplete(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
	if (bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Game saved to slot %s index %i"), *SlotName, UserIndex);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Game failed to save!"));
	}
}

void UGrappleGameInstanceBase::SettingsSaveComplete(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
	if (bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Settings saved to slot %s index %i"), *SlotName, UserIndex);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Settings failed to save!"));
	}
}
