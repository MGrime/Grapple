// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacterBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
UCLASS()
class GRAPPLE_API APlayerCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Input functions called by controller
	void MoveForwards(float Value);
	void Strafe(float Value);
	void LookVertical(float Value);
	void LookHorizontal(float Value);
	void SprintPress();
	void SprintRelease();

private:

	UPROPERTY(EditAnywhere, Category = "Camera")
		USpringArmComponent* SpringArm;	
	
	UPROPERTY(EditAnywhere, Category = "Camera")
		UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
		float WalkingSpeed;

	UPROPERTY(EditAnywhere)
		float SprintSpeed;

	UPROPERTY()
		bool bSprinting;

	UPROPERTY()
		float SprintWindupTime = 1.0f;

	UPROPERTY()
		float CurrentSprintHeldTime = 0.0f;

};


