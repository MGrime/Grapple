// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyProjectileBase.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AEnemyProjectileBase::AEnemyProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create default components

	// Collision volume for on hit code
	LaserCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Volume"));
	RootComponent = LaserCollision;
	
	// Laser mesh to represent laser with fancy material
	LaserMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Laser Mesh"));
	LaserMesh->SetupAttachment(LaserCollision);

	// Create projectile movement component
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Component"));

	// Configure
	MovementComponent->InitialSpeed = MovementSpeed;
	MovementComponent->MaxSpeed = MovementSpeed;
	MovementComponent->ProjectileGravityScale = 0.0f;
	InitialLifeSpan = 10.0f;
	

	

}

// Called when the game starts or when spawned
void AEnemyProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

