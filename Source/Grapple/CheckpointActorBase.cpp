// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckpointActorBase.h"
#include "PlayerCharacterBase.h"

#include "Components/BoxComponent.h"

// Sets default values
ACheckpointActorBase::ACheckpointActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create the area to interact
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Volume"));
	SetRootComponent(TriggerBox);

	// Hook dynamic event to mark checkpoint 
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACheckpointActorBase::TriggerInteraction);


}

void ACheckpointActorBase::TriggerInteraction(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto Player = Cast<APlayerCharacterBase>(OtherActor);

	// Cast works therefore we have interacted with the player
	if (Player)
	{
		bIsChecked = true;
	}
	
}

