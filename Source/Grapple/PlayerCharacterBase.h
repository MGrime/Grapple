// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacterBase.generated.h"

// Declare enumerations for wall running

// Mark and know which side the wall we are attached to is on
UENUM(BlueprintType)
enum class EWallRunSide : uint8
{
	Left = 0 UMETA(DisplayName = "Left"),
	Right = 1 UMETA(DisplayName = "Right"),
};

// Mark and know why we stopped wall running
UENUM()
enum class EWallRunEndReason
{
	FallOff = 0 UMETA(DisplayName = "Fall Off"),
	JumpOff = 1 UMETA(DisplayName = "Jump Off"),
};

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
	void JumpPress();
	void JumpRelease();
	void CrouchToggle();
	void Punch();

	#pragma region ANIMATION
	UFUNCTION(BlueprintPure, Category = "Wall Running")
		bool IsWallRunning();

	UFUNCTION(BlueprintPure, Category = "Wall Running")
		EWallRunSide GetWallRunningSide();

	UFUNCTION(BlueprintPure, Category = "Crouching")
		bool IsCrouching();

	UFUNCTION(BlueprintPure, Category = "Landed")
		bool HasJustLanded();

	UFUNCTION(BlueprintPure, Category = "Attacking")
		bool IsPunching();

	UFUNCTION()
		void NotifyCompletedAnimation(UAnimSequenceBase* CompletedAnimation);

	UFUNCTION()
		void NotifyAnimationEvent(FString EventData);

	// Returns true if jumps left < max jumps. If equal it is our first jump
	UFUNCTION(BlueprintPure)
		bool IsSecondJump();
	#pragma endregion

	#pragma region IK
	UFUNCTION(BlueprintCallable, Category = "IK")
		float GetRightFootIK();
	
	UFUNCTION(BlueprintCallable, Category = "IK")
		float GetLeftFootIK();
	#pragma endregion

	#pragma region WALL RUN

	// Public so other events can end wall running (i.e. getting hit)
	UFUNCTION(Category = "Wall Running")
		void EndWallRun(EWallRunEndReason Reason);
	
	#pragma endregion

private:
	#pragma region CAMERA
	
	UPROPERTY(EditAnywhere, Category = "Camera")
		USpringArmComponent* SpringArm;	
	
	UPROPERTY(EditAnywhere, Category = "Camera")
		UCameraComponent* Camera;

	#pragma endregion 

	#pragma region MOVEMENT

	UPROPERTY(EditAnywhere)
		float CrouchSpeed;

	UPROPERTY(EditAnywhere)
		float WalkingSpeed;

	UPROPERTY(EditAnywhere)
		float SprintSpeed;

	UPROPERTY(EditAnywhere)
		bool bHasJustLanded;

	// True play right footstep, false play left footstep. easy peasy
	UPROPERTY(EditAnywhere)
		bool bIsRightFoot;

	UPROPERTY(EditAnywhere)
		USoundBase* RightFootstep;

	UPROPERTY(EditAnywhere)
		USoundBase* LeftFootstep;

	// Track the last sound to fix any lagging on jump
	UPROPERTY()
		UAudioComponent* LastPlayedSound;
	
	#pragma endregion

	#pragma region ATTACKING

	UPROPERTY(EditAnywhere)
		bool bPunching;

	UPROPERTY(EditAnywhere)
		USoundBase* PunchingSound;
	
	#pragma endregion

	#pragma region SPRINT

	UPROPERTY()
		bool bSprinting;

	UPROPERTY(EditAnywhere)
		float SprintWindupTime = 0.3f;

	UPROPERTY()
		float CurrentSprintHeldTime = 0.0f;
	
	#pragma endregion

	#pragma region CROUCHING

	UPROPERTY(VisibleAnywhere, Category = "Crouching")
		bool bIsCrouching;

	UPROPERTY(EditAnywhere, Category = "Crouching")
		float CrouchJumpPowerMultiplier;

	UPROPERTY()
		float BaseJumpPower;
	
	
	#pragma endregion

	#pragma region WALL RUNNING

	#pragma region VARIABLES
	
	UPROPERTY(VisibleAnywhere, Category = "Wall Running")
		FVector WallRunDirection;

	UPROPERTY(VisibleAnywhere, Category = "Wall Running")
		bool bIsWallRunning;

	UPROPERTY(VisibleAnywhere, Category = "Wall Running")
		int JumpsLeft;

	UPROPERTY(EditAnywhere, Category = "Wall Running")
		int MaxJumps;

	// For animation
	UPROPERTY(VisibleAnywhere, Category = " Wall Running")
		bool bSecondJump;

	UPROPERTY(EditAnywhere, Category = "Wall Running")
		float RightAxisValue;

	UPROPERTY(EditAnywhere, Category = "Wall Running")
		float ForwardAxisValue;

	UPROPERTY(VisibleAnywhere, Category = "Wall Running")
		EWallRunSide WallRunSide;

	#pragma endregion

	#pragma region FUNCTIONS

	// Helpers
	// Call when we start moving forward to init
	UFUNCTION(BlueprintCallable, Category = "Wall Running")
		void MovingForward();

	// Call when we want to jump. Returns true if a jump was consumed, false if not
	UFUNCTION(BlueprintCallable, Category = "Wall Running")
		bool ConsumeJump();

	// Call on wall run start to work out orientation etc
	UFUNCTION(BlueprintCallable, Category = "Wall Running")
		void CalculateRunDirectionAndSide(FVector WallNormal);

	// Check if the surface is wall runnable
	UFUNCTION(BlueprintCallable, Category = "Wall Running")
		bool CanSurfaceBeWallRan(FVector SurfaceNormal);

	// Calculate launch velocity when we jump off
	UFUNCTION(BlueprintCallable, Category = "Wall Running")
		FVector FindLaunchVelocity();

	// Checks for input to make sure we can still wall ride after we have already started
	UFUNCTION(BlueprintCallable, Category = "Wall Running")
		bool AreRequiredKeysDown();

	// Set our horizontal velocity
	UFUNCTION(BlueprintCallable, Category = "Wall Running")
		void SetHorizontalVelocity(FVector2D newHorizontalVelocity);

	// Clamp velocity
	UFUNCTION(BlueprintCallable, Category = "Wall Running")
		void ClampHorizontalVelocity();
	
	// Overridden from base class

	virtual void Landed(const FHitResult& Hit) override;

	// Custom events
	
	UFUNCTION(Category = "Wall Running")
		void ResetJump(int ResetToJumps);

	UFUNCTION(Category = "Wall Running")
		void BeginWallRun();

	UFUNCTION(Category = "Wall Running")
		void UpdateWallRun();

	// Custom Delgates for hooking into compoentns

	UFUNCTION(Category = "Wall Running")
	void CheckWallOnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	
	#pragma endregion
		
	#pragma endregion

	#pragma region INVERSE KINEMATICS

	// Z-Height of the character
	UPROPERTY()
		float IKScale;

	// Half height of the capsule
	UPROPERTY()
		float IKTraceDistance;

	// Set in blueprint
	UPROPERTY(EditAnywhere, Category="IK")
		FName IKRightFootSocket;

	UPROPERTY(EditAnywhere, Category = "IK")
		FName IKLeftFootSocket;

	UPROPERTY(EditAnywhere, Category = "IK")
		float IKInteropSpeed;

	// Updated per frame
	UPROPERTY()
		float IKRightFoot;
	UPROPERTY()
		float IKLeftFoot;

	UFUNCTION()
		void UpdateIKVariables(float DeltaTime);
	
	// Used by main update function
	UFUNCTION()
		float IKFootTrace(const FName& Socket);

	#pragma endregion
	
	#pragma region GAME MANAGEMENT

	virtual void FellOutOfWorld(const UDamageType& dmgType) override;

	#pragma endregion

};


