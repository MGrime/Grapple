// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "EnemyProjectileBase.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
UCLASS()
class GRAPPLE_API AEnemyProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Laser mesh to represent visually the laser
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* LaserMesh;

	// Collision volume to hit the player
	UPROPERTY(EditAnywhere)
		USphereComponent* LaserCollision;

	// Projectile movement component to control movement easily
	UPROPERTY(EditAnywhere)
		UProjectileMovementComponent* MovementComponent;

	#pragma region EDITOR CONFIG VARIABLEs

	UPROPERTY(EditAnywhere)
		float MovementSpeed = 1200.0f;
	
	#pragma endregion

	#pragma region FUNCTIONS

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	#pragma endregion

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};


