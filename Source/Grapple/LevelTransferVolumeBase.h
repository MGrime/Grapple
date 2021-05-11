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

	UFUNCTION(BlueprintCallable)
		FName GetLevelTransferName();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
		UBoxComponent* TriggerBox;

	// This is the level the box will take you to
	UPROPERTY(EditAnywhere)
		FName LevelTransferName;

	// The index of the level this box is placed in
	UPROPERTY(EditAnywhere)
		int32 LevelPlacedIndex;

	UPROPERTY(EditAnywhere)
		USoundBase* SuccessSound;

	UPROPERTY(EditAnywhere)
		USoundBase* ErrorSound;

	// Boolean accessed by HUD blueprint to pulse coin text on error
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bPulseErrorText;

	UFUNCTION()
	void TriggerInteraction(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	// Gathered at runtime
	// Only transfer after > 3/4 of the tokens have been collected
	UPROPERTY()
		int32 TokensInLevel;
};
