// Fill out your copyright notice in the Description page of Project Settings.


#include "FinishedAnimNotify.h"

#include "PlayerCharacterBase.h"
#include "Kismet/GameplayStatics.h"

void UFinishedAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UE_LOG(LogTemp, Warning, TEXT("Notfiying!"));

	const auto Player = MeshComp->GetOwner();
	
	if (Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("Got player!"));
		const auto DowncastPlayer = Cast<APlayerCharacterBase>(Player);

		if (DowncastPlayer)
		{

			UE_LOG(LogTemp, Warning, TEXT("Cast player!"));
			DowncastPlayer->NotifyCompletedAnimation(Animation);
		}
	}
}
