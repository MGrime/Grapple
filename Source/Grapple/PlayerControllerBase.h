// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBase.generated.h"

class APlayerCharacterBase;
/**
 * 
 */
UCLASS()
class GRAPPLE_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	virtual void SetupInputComponent() override;

	// Movement functions
	void CallMoveForwards(float Value);
	void CallStrafe(float Value);
	
	void CallLookVertical(float Value);
	void CallLookHorizontal(float Value);
	
	void CallJumpPress();
	void CallJumpRelease();

	void CallSprintPress();
	void CallSprintRelease();

	void CallCrouchToggle();

	// Changes between the main menu and the options
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ToggleMainMenuLoaded();


private:
	UPROPERTY()
		APlayerCharacterBase* PlayerChar;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> MainMenuClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> OptionsClass;

	UPROPERTY(VisibleAnywhere)
		UUserWidget* MainMenuWidget;

	UPROPERTY(VisibleAnywhere)
		UUserWidget* OptionMenuWidget;
};
