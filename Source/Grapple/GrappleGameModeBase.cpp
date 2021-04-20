// Copyright Epic Games, Inc. All Rights Reserved.


#include "GrappleGameModeBase.h"

#include "PlayerControllerBase.h"
#include "Kismet/GameplayStatics.h"

void AGrappleGameModeBase::LoadLevel(FName LevelName)
{
	const auto World = GetWorld();

	if (World)
	{
		UGameplayStatics::OpenLevel(World, LevelName, TRAVEL_Absolute);
	}

}
