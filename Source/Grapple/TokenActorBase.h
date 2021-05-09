// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/RotatingMovementComponent.h"

#include "TokenActorBase.generated.h"

class USphereComponent;
UCLASS()
class GRAPPLE_API ATokenActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATokenActorBase();

private:
	UPROPERTY(EditAnywhere)
		URotatingMovementComponent* RotationComponent;
	
	UPROPERTY(EditAnywhere)
		USphereComponent* TriggerBox;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
		USceneComponent* CoinRootRotation;

	UPROPERTY(EditAnywhere)
		USoundBase* PickupSound;

	UFUNCTION()
		void TriggerInteraction(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
