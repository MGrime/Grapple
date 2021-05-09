// Fill out your copyright notice in the Description page of Project Settings.


#include "EventAnimNotify.h"

#include "PlayerCharacterBase.h"

void UEventAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	const auto Player = MeshComp->GetOwner();

	if (Player)
	{
		const auto DowncastPlayer = Cast<APlayerCharacterBase>(Player);
		if (DowncastPlayer)
		{
			DowncastPlayer->NotifyAnimationEvent(EventData);
		}
	}
}
