// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelTransferVolumeBase.generated.h"

class UBoxComponent;
UCLASS()
class GRAPPLE_API ALevelTransferVolumeBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelTransferVolumeBase();

protected:
	UPROPERTY(EditAnywhere)
		UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere)
		FName LevelName;

	UFUNCTION()
	void TriggerInteraction(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
