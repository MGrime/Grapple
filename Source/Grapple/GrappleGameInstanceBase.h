// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GrappleGameInstanceBase.generated.h"

/**
 * 
 */
UCLASS()
class GRAPPLE_API UGrappleGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	bool bIsMainMenu = true;
};
