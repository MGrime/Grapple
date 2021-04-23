// Copyright Epic Games, Inc. All Rights Reserved.


#include "GrappleGameModeBase.h"

#include "PlayerControllerBase.h"
#include "Kismet/GameplayStatics.h"

void AGrappleGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// Try to load settings
	SettingsSaveGame = Cast<USettingsSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("SettingsSaveGame"), 0));
	if (!SettingsSaveGame)
	{
		// Nothhing loaded so create blank
		SettingsSaveGame = Cast<USettingsSaveGame>(UGameplayStatics::CreateSaveGameObject(USettingsSaveGame::StaticClass()));
	}

}

void AGrappleGameModeBase::LoadLevel(FName LevelName)
{
	const auto World = GetWorld();

	if (World)
	{
		UGameplayStatics::OpenLevel(World, LevelName, TRAVEL_Absolute);
	}

}

void AGrappleGameModeBase::SaveSettings()
{
	// Bind callback
	FAsyncSaveGameToSlotDelegate SavedDelegate;
	SavedDelegate.BindUObject(this, &AGrappleGameModeBase::SettingsSaveComplete);

	// Async save
	UGameplayStatics::AsyncSaveGameToSlot(SettingsSaveGame, SettingsSaveGame->SaveSlotName, SettingsSaveGame->UserIndex, SavedDelegate);
}

USettingsSaveGame* AGrappleGameModeBase::GetSaveGame()
{
	return SettingsSaveGame;
}

void AGrappleGameModeBase::SettingsSaveComplete(const FString& SlotName, const int32 UserIndex, bool bSuccess)
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
