// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CheckpointActorBase.generated.h"

class UBoxComponent;
UCLASS()
class GRAPPLE_API ACheckpointActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckpointActorBase();

	// If the check point has been activated or not
	UPROPERTY(EditAnywhere)
		bool bIsChecked = false;

	// Order in level
	UPROPERTY(EditAnywhere)
		int32 CheckpointIndex;

	// The area to trigger the box
	UPROPERTY(EditAnywhere)
		UBoxComponent* TriggerBox;
	
protected:
	UFUNCTION()
	void TriggerInteraction(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
