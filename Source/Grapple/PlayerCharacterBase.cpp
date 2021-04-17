// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterBase.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
APlayerCharacterBase::APlayerCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set the max walk speed
	GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;

	// Create default subobjects for editor
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Third Person Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	
}

// Called when the game starts or when spawned
void APlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// We have pressed sprint and not let go yet
	if (bSprinting)
	{
		// If we havent reached max wind up time
		if (!(CurrentSprintHeldTime + DeltaTime > SprintWindupTime))
		{
			// Increase time
			CurrentSprintHeldTime += DeltaTime;

			// Use LERP to set movement speed
			GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed * (1.0 - CurrentSprintHeldTime) + SprintSpeed * CurrentSprintHeldTime;
		}
	}
	else
	{
		// We have just let go so decrease time
		if (CurrentSprintHeldTime > 0.0f)
		{
			// Decrease
			CurrentSprintHeldTime -= DeltaTime;

			// User lerp to set movement speed
			GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed * (1.0 - CurrentSprintHeldTime) + SprintSpeed * CurrentSprintHeldTime;
		}
		else if (CurrentSprintHeldTime < 0.0f)
		{
			// Catch underflow
			CurrentSprintHeldTime = 0.0f;

			// Fix move speed
			GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
		}
	}
}

// Called to bind functionality to input
void APlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerCharacterBase::MoveForwards(float Value)
{
	AddMovementInput(Value * GetActorForwardVector());
}

void APlayerCharacterBase::Strafe(float Value)
{
	AddMovementInput(Value * GetActorRightVector());
}

void APlayerCharacterBase::LookVertical(float Value)
{
	AddControllerPitchInput(Value);
}

void APlayerCharacterBase::LookHorizontal(float Value)
{
	AddControllerYawInput(Value);
}

void APlayerCharacterBase::SprintPress()
{
	bSprinting = true;
}

void APlayerCharacterBase::SprintRelease()
{
	bSprinting = false;
}



