// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTransferVolumeBase.h"

#include "GrappleGameInstanceBase.h"
#include "GrappleGameModeBase.h"
#include "GrappleGameStateBase.h"
#include "LevelDetailsActor.h"
#include "PlayerCharacterBase.h"
#include "PlayerControllerBase.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

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

FName ALevelTransferVolumeBase::GetLevelTransferName()
{
	return LevelTransferName;
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
				// get the instance and state
				const auto GameInstance = World->GetGameInstance<UGrappleGameInstanceBase>();
				const auto GameState = World->GetGameState<AGrappleGameStateBase>();
				const auto LevelDetails = Cast<ALevelDetailsActor>(UGameplayStatics::GetActorOfClass(World, ALevelDetailsActor::StaticClass()));
				// Add to the save game
				// Check if the level times already exists
				if (GameInstance->ActiveSaveGame->LevelTimes.Contains(LevelPlacedIndex))
				{
					// Only update if time is better
					if (GameInstance->ActiveSaveGame->LevelTimes[LevelPlacedIndex].Time > GameState->LevelTimer)
					{
						GameInstance->ActiveSaveGame->LevelTimes[LevelPlacedIndex].Time = GameState->LevelTimer;
					}
				}
				else
				{
					// First time so this is our best time
					GameInstance->ActiveSaveGame->LevelTimes.Add(LevelPlacedIndex, FLevelTimeData{ GameState->LevelTimer });
				}
				// Check for tokens count
				if (GameInstance->ActiveSaveGame->LevelTokens.Contains(LevelPlacedIndex))
				{
					if (GameInstance->ActiveSaveGame->LevelTokens[LevelPlacedIndex].TokensCollected < GameState->LevelTokens)
					{
						GameInstance->ActiveSaveGame->LevelTokens[LevelPlacedIndex] = FLevelTokenData{ GameState->LevelTokens,LevelDetails->TotalTokenCount };
					}
				}
				else
				{
					// First time so this is the most we have got
					GameInstance->ActiveSaveGame->LevelTokens.Add(LevelPlacedIndex, FLevelTokenData{ GameState->LevelTokens,LevelDetails->TotalTokenCount });
				}
				
				// Save the game
				GameInstance->SaveActive();

				// Disable game counter
				GameState->bIsLevelDone = true;

				// Call toggle ui on player
				Player->GetController<APlayerControllerBase>()->LoadLevelCompleteUI();
			}
		}
	}
	
}
