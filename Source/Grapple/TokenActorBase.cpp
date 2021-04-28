// Fill out your copyright notice in the Description page of Project Settings.


#include "TokenActorBase.h"

#include "GrappleGameStateBase.h"
#include "PlayerCharacterBase.h"
#include "Components/SphereComponent.h"

// Sets default values
ATokenActorBase::ATokenActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CoinRootRotation = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(CoinRootRotation);

	// Create mesh default and box default
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(CoinRootRotation);

	TriggerBox = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger Volume"));
	TriggerBox->SetupAttachment(Mesh);

	// HJook dynamic event
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATokenActorBase::TriggerInteraction);

	// Create actor comps
	RotationComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotation Component"));
	

}

void ATokenActorBase::TriggerInteraction(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto Player = Cast<APlayerCharacterBase>(OtherActor);
	// The cast works so player must have been the collider
	if (Player)
	{
		// Get reference to the world
		const auto World = GetWorld();
		if(World)
		{
			const auto GameState = World->GetGameState<AGrappleGameStateBase>();

			GameState->LevelTokens++;

			Destroy();
		}
	}
}

