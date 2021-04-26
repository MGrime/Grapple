// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleGameStateBase.h"

AGrappleGameStateBase::AGrappleGameStateBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGrappleGameStateBase::BeginPlay()
{
	Super::BeginPlay();
	
	LevelTimer = 0.0f;
	LevelTokens = 0;
	bIsLevelDone = false;
}

void AGrappleGameStateBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (!bIsLevelDone)
	{
		LevelTimer += DeltaSeconds;
	}
}
