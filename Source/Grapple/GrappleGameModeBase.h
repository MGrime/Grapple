// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GrappleGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class GRAPPLE_API AGrappleGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		void LoadLevel(FName LevelName);
	
};
