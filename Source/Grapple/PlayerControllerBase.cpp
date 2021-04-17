// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"

#include "PlayerCharacterBase.h"

void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	PlayerChar = Cast<APlayerCharacterBase>(GetPawn());
}

void APlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);

	// Bind functions
	InputComponent->BindAxis(TEXT("Move"), this, &APlayerControllerBase::CallMoveForwards);
	InputComponent->BindAxis(TEXT("Strafe"), this, &APlayerControllerBase::CallStrafe);
	InputComponent->BindAxis(TEXT("Look Vertical"), this, &APlayerControllerBase::CallLookVertical);
	InputComponent->BindAxis(TEXT("Look Horizontal"), this, &APlayerControllerBase::CallLookHorizontal);
}

void APlayerControllerBase::CallMoveForwards(float Value)
{
	if (PlayerChar)
	{
		PlayerChar->MoveForwards(Value);
	}
}

void APlayerControllerBase::CallStrafe(float Value)
{
	if (PlayerChar)
	{
		PlayerChar->Strafe(Value);
	}
}

void APlayerControllerBase::CallLookVertical(float Value)
{
	if (PlayerChar)
	{
		PlayerChar->LookVertical(Value);
	}
}

void APlayerControllerBase::CallLookHorizontal(float Value)
{
	if (PlayerChar)
	{
		PlayerChar->LookHorizontal(Value);
	}
}

void APlayerControllerBase::CallJump()
{
	if (PlayerChar)
	{
		PlayerChar->Jump();
	}
}
