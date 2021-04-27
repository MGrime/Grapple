// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleGameInstanceBase.h"

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
