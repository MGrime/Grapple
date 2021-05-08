// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"

#include "GrappleGameInstanceBase.h"
#include "PlayerCharacterBase.h"
#include "Blueprint/UserWidget.h"

void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	PlayerChar = Cast<APlayerCharacterBase>(GetPawn());

	if (PlayerChar)
	{
		const auto GameInstance = GetGameInstance<UGrappleGameInstanceBase>();

		if (GameInstance)
		{
			if (GameInstance->bIsMainMenu)
			{
				MainMenuWidget = CreateWidget(this, MainMenuClass);
				OptionMenuWidget = CreateWidget(this, OptionsClass);

				if (MainMenuWidget)
				{
					MainMenuWidget->AddToViewport();
					UE_LOG(LogTemp, Warning, TEXT("Created menu widget!"));
				}
			}
			else if (GameInstance->bIsLevelSelect)
			{
				LevelSelectWidget = CreateWidget(this, LevelSelectClass);

				if (LevelSelectWidget)
				{
					LevelSelectWidget->AddToViewport();
					UE_LOG(LogTemp, Warning, TEXT("Created level select widget!"));
				}
				
			}

			if (GameInstance->bIsMainMenu || GameInstance->bIsLevelSelect)
			{
				// Set UI input mode
				SetInputMode(FInputModeUIOnly());
				SetShowMouseCursor(true);
			}
			else
			{
				SetInputMode(FInputModeGameOnly());
				SetShowMouseCursor(false);
			}

			bIsIgnoringInput = false;
		}		
	}
}

void APlayerControllerBase::Tick(float DeltaSeconds)
{
	// Try to reget player when its invalid
	if (!IsValid(PlayerChar))
	{
		PlayerChar = Cast<APlayerCharacterBase>(GetPawn());
	}
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

	InputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &APlayerControllerBase::CallSprintPress);
	InputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &APlayerControllerBase::CallSprintRelease);

	InputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APlayerControllerBase::CallJumpPress);
	InputComponent->BindAction(TEXT("Jump"), IE_Released, this, &APlayerControllerBase::CallJumpRelease);

	InputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &APlayerControllerBase::CallCrouchToggle);
}

void APlayerControllerBase::CallMoveForwards(float Value)
{
	if (PlayerChar && !bIsIgnoringInput)
	{
		PlayerChar->MoveForwards(Value);
	}
}

void APlayerControllerBase::CallStrafe(float Value)
{
	if (PlayerChar && !bIsIgnoringInput)
	{
		PlayerChar->Strafe(Value);
	}
}

void APlayerControllerBase::CallLookVertical(float Value)
{
	if (PlayerChar && !bIsIgnoringInput)
	{
		PlayerChar->LookVertical(Value);
	}
}

void APlayerControllerBase::CallLookHorizontal(float Value)
{
	if (PlayerChar && !bIsIgnoringInput)
	{
		PlayerChar->LookHorizontal(Value);
	}
}

void APlayerControllerBase::CallJumpPress()
{
	if (PlayerChar && !bIsIgnoringInput)
	{
		PlayerChar->JumpPress();
	}
}

void APlayerControllerBase::CallJumpRelease()
{
	if (PlayerChar && !bIsIgnoringInput)
	{
		PlayerChar->JumpRelease();
	}
}


void APlayerControllerBase::CallSprintPress()
{
	if (PlayerChar && !bIsIgnoringInput)
	{
		PlayerChar->SprintPress();
	}
}

void APlayerControllerBase::CallSprintRelease()
{
	if (PlayerChar && !bIsIgnoringInput)
	{
		PlayerChar->SprintRelease();
	}
}

void APlayerControllerBase::CallCrouchToggle()
{
	if (PlayerChar && !bIsIgnoringInput)
	{
		PlayerChar->CrouchToggle();
	}
}

void APlayerControllerBase::ToggleMainMenuLoaded()
{
	if (MainMenuWidget && OptionMenuWidget)
	{
		if (MainMenuWidget->IsInViewport())
		{
			MainMenuWidget->RemoveFromViewport();
			OptionMenuWidget->AddToViewport();
		}
		else
		{
			OptionMenuWidget->RemoveFromViewport();
			MainMenuWidget->AddToViewport();
		}
	}
}

void APlayerControllerBase::LoadLevelCompleteUI()
{
	const auto LevelCompleteUI = CreateWidget(this, LevelCompleteClass);

	if (LevelCompleteUI)
	{	
		LevelCompleteUI->AddToViewport();
		// Set UI input mode
		SetInputMode(FInputModeUIOnly());
		SetShowMouseCursor(true);

		bIsIgnoringInput = true;
	}
}
