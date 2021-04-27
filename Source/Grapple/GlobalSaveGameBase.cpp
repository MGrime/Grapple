// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalSaveGameBase.h"

UGlobalSaveGameBase::UGlobalSaveGameBase()
{
	SaveSlotName = TEXT("SaveSlotOne");
	UserIndex = 0;
}
// If it is in level times, it will be in tokens aswell
bool UGlobalSaveGameBase::HasLevelEntry(int32 levelIndex)
{
	return LevelTimes.Contains(levelIndex);
}

float UGlobalSaveGameBase::GetLevelTime(int32 levelIndex)
{
	if (HasLevelEntry(levelIndex))
	{
		return LevelTimes[levelIndex].Time;
	}
	return -1.0f;
}

int32 UGlobalSaveGameBase::GetLevelTokensCollected(int32 levelIndex)
{
	if (HasLevelEntry(levelIndex))
	{
		return LevelTokens[levelIndex].TokensCollected;
	}
	return -1;
}

int32 UGlobalSaveGameBase::GetLevelTokensMax(int32 levelIndex)
{
	if (HasLevelEntry(levelIndex))
	{
		return LevelTokens[levelIndex].TokensInLevel;
	}
	return -1;
}
