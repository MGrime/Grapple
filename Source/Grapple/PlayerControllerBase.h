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
public:
	APlayerControllerBase();
	
	// Called when level is ended
	UFUNCTION(BlueprintCallable, Category = "UI")
		void LoadLevelCompleteUI();
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

	void CallPunch();

	void CallPause();

	
	// Changes between the main menu and the options
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ToggleMainMenuLoaded();

	// Called by blueprint not input
	UFUNCTION(BlueprintCallable, Category = "UI")
	void Unpause();


private:
	UPROPERTY()
		APlayerCharacterBase* PlayerChar;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> MainMenuClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> OptionsClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> LevelSelectClass;

	// Has no accompying pointer as it gets made and set to viewport, then immedately destoryed when new level loaded.
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> LevelCompleteClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> PauseClass;

	UPROPERTY(VisibleAnywhere)
		UUserWidget* MainMenuWidget;

	UPROPERTY(VisibleAnywhere)
		UUserWidget* OptionMenuWidget;

	UPROPERTY(VisibleAnywhere)
		UUserWidget* LevelSelectWidget;

	UPROPERTY(VisibleAnywhere)
		UUserWidget* PauseWidget;

	UPROPERTY()
		bool bIsIgnoringInput;
};
