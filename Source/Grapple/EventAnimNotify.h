// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EventAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class GRAPPLE_API UEventAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	UFUNCTION()
		virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	// String to store what event is happening
	UPROPERTY(EditAnywhere)
		FString EventData;
};
