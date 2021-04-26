// Copyright Epic Games, Inc. All Rights Reserved.


#include "GrappleGameModeBase.h"

#include "PlayerControllerBase.h"
#include "Kismet/GameplayStatics.h"
#include "LevelDetailsActor.h"

void AGrappleGameModeBase::BeginPlay()
{
	// Search for level detials actor
	ActiveLevelDetails = Cast<ALevelDetailsActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelDetailsActor::StaticClass()));
	
}

FString AGrappleGameModeBase::GetActiveLevelName()
{
	if (ActiveLevelDetails)
	{
		return ActiveLevelDetails->LevelName;
	}
	return TEXT("");
}

float AGrappleGameModeBase::GetActiveLevelGoldTime()
{
	if (ActiveLevelDetails)
	{
		return ActiveLevelDetails->TimeForGold;
	}
	return 0.0f;
}

void AGrappleGameModeBase::LoadLevel(FName LevelName)
{
	const auto World = GetWorld();

	if (World)
	{
		UGameplayStatics::OpenLevel(World, LevelName, TRAVEL_Absolute);
	}

}
