// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTransferVolumeBase.h"

#include "GrappleGameModeBase.h"
#include "PlayerCharacterBase.h"
#include "Components/BoxComponent.h"

// Sets default values
ALevelTransferVolumeBase::ALevelTransferVolumeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Volume"));
	SetRootComponent(TriggerBox);

	// Hook dynamic event to change level
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ALevelTransferVolumeBase::TriggerInteraction);

}

void ALevelTransferVolumeBase::TriggerInteraction(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	UE_LOG(LogTemp, Warning, TEXT("Collision Detected!"));
	
	const auto Player = Cast<APlayerCharacterBase>(OtherActor);
	// The cast works so player must have been the collider
	if (Player)
	{
		const auto World = GetWorld();

		if (World)
		{
			const auto GameMode = World->GetAuthGameMode<AGrappleGameModeBase>();

			if (GameMode)
			{
				GameMode->LoadLevel(LevelName);
			}
		}
	}
	
}
