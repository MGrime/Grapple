// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterBase.h"

#include "CheckpointActorBase.h"
#include "EnemyCharacterBase.h"
#include "GrappleGameModeBase.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerCharacterBase::APlayerCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set the max walk speed
	GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;

	// Hook on our custom on hit event
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &APlayerCharacterBase::CheckWallOnHit);

	// Create default subobjects for editor
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Third Person Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// Set defaults
	// Standard is two jumps
	MaxJumps = 2;

	bIsCrouching = false;

	bSecondJump = false;
	
	bPunching = false;
}

// Called when the game starts or when spawned
void APlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// Initialise how many jumps
	JumpsLeft = MaxJumps;

	// Setup movement constriants to help with wall running calcs later
	const auto CharacterMovementRef = GetCharacterMovement();
	CharacterMovementRef->SetPlaneConstraintEnabled(true);

	// Init the IK Variables
	IKScale = GetActorTransform().GetScale3D().Z;

	IKTraceDistance = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() * IKScale;

	
	BaseJumpPower = CharacterMovementRef->JumpZVelocity;
	GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
}

// Called every frame
void APlayerCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// We use it a lot in this tick
	const auto CharacterMovementRef = GetCharacterMovement();
	
	#pragma region SPRINT
	/* Sprint management - This code section deals with the player sprinting. It uses a windup system to mimic analog input*/
	if (!CharacterMovementRef->IsFalling())
	{
		// We have pressed sprint and not let go yet
		if (bSprinting)
		{
			// If we havent reached max wind up time
			if (!(CurrentSprintHeldTime + DeltaTime > SprintWindupTime))
			{
				// Increase time
				CurrentSprintHeldTime += DeltaTime;

				// Use LERP to set movement speed
				const float percentageWound = CurrentSprintHeldTime / SprintWindupTime;

				GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed * (1.0 - percentageWound) + SprintSpeed * percentageWound;
			}
		}
		else
		{
			// We have just let go so decrease time
			if (CurrentSprintHeldTime > 0.0f)
			{
				// Decrease
				CurrentSprintHeldTime -= DeltaTime;

				// Use lerp to set movement speed
				const float percentageWound = CurrentSprintHeldTime / SprintWindupTime;

				GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed * (1.0 - percentageWound) + SprintSpeed * percentageWound;
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

	#pragma endregion

	#pragma region WALL RUNNING

	// Make sure our horizontal velocity is normalised
	ClampHorizontalVelocity();

	// Check if we are wall running
	if (bIsWallRunning)
	{
		// Update the wall run
		UpdateWallRun();
	}
	
	#pragma endregion

	#pragma region PUNCHING

	if (bPunching)
	{
		// Get socket location of fist
		const auto FistLocation = GetMesh()->GetSocketLocation(FName(TEXT("RightHand")));

		// Capsule trace for objects
		const auto World = GetWorld();
		if (World)
		{
			// To store the hit actor
			FHitResult OutHit;
			// We only want to collide with pawns
			const auto ObjectsToCollide = FCollisionObjectQueryParams(ECC_Pawn);
			// Trace to find the first hit object
			const auto HasHitConnected = World->SweepSingleByObjectType(
				OutHit,
				FistLocation,
				FistLocation,
				FQuat::Identity,
				ObjectsToCollide,
				FCollisionShape::MakeCapsule(50.0f, 50.0f)
			);

			if (HasHitConnected)
			{
				// This means we have hit an enemy if the cast works
				const auto Enemy = Cast<AEnemyCharacterBase>(OutHit.Actor);
				if (Enemy)
				{
					if (IsValid(Enemy))
					{
						Enemy->ReceivePunch();
					}
				}
			}
		}
		
		
	}
	
	
	#pragma endregion

	// Update inverse kinematics
	UpdateIKVariables(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerCharacterBase::MoveForwards(float Value)
{
	if (!bPunching)
	{
		// Save the value
		ForwardAxisValue = Value;

		AddMovementInput(Value * GetActorForwardVector());
	}
}

void APlayerCharacterBase::Strafe(float Value)
{
	if (!bPunching)
	{
		// Save the value
		RightAxisValue = Value;

		AddMovementInput(Value * GetActorRightVector());
	}
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

	// Disable crouch if start to sprint
	if (bIsCrouching)
	{
		CrouchToggle();
	}
}

void APlayerCharacterBase::SprintRelease()
{
	bSprinting = false;
}

void APlayerCharacterBase::JumpPress()
{
	// Call to consume a jump and check if we have or not
	if (ConsumeJump())
	{
		// We have consumed a jump

		// Track for animation
		if (JumpsLeft == 0)
		{
			bSecondJump = true;
		}
		else
		{
			bSecondJump = false;
		}
		
		// Launch the character
		const FVector LaunchVel = FindLaunchVelocity();
		LaunchCharacter(LaunchVel,false,true);

		// Check if we have been wall running
		if (bIsWallRunning)
		{
			// Call end event
			EndWallRun(EWallRunEndReason::JumpOff);
		}

		// Or if we have been crouching
		if (bIsCrouching)
		{
			// Disable crouch after the jump
			CrouchToggle();
		}

		// Cancel any footsteps
		if (LastPlayedSound)
		{
			if (LastPlayedSound->IsPlaying())
			{
				LastPlayedSound->Stop();
			}
		}
	}
}

void APlayerCharacterBase::JumpRelease()
{
	StopJumping();
}

void APlayerCharacterBase::CrouchToggle()
{
	// Can't crouch while wall running
	if (bIsWallRunning)
	{
		return;
	}

	if (bSprinting)
	{
		// Coming out a sprint will slide
		
	}

	bIsCrouching = !bIsCrouching;

	if (bIsCrouching)
	{
		// Reduce max walk speed and slightly increase jump power
		const auto& CharacterMovementRef = GetCharacterMovement();

		CharacterMovementRef->JumpZVelocity = BaseJumpPower * CrouchJumpPowerMultiplier;
		CharacterMovementRef->MaxWalkSpeed = CrouchSpeed;
	}
	else
	{

		// Reduce max walk speed and slightly increase jump power
		const auto& CharacterMovementRef = GetCharacterMovement();

		CharacterMovementRef->JumpZVelocity = BaseJumpPower;
		CharacterMovementRef->MaxWalkSpeed = WalkingSpeed;
	}
	
	
	
}
void APlayerCharacterBase::Punch()
{
	// Can only punch if we arent already, arent crouching and arent wall running
	if (!bPunching && !bIsCrouching && !bIsWallRunning)
	{
		bPunching = true;
	}
}

bool APlayerCharacterBase::IsCrouching()
{
	return bIsCrouching;
}

bool APlayerCharacterBase::HasJustLanded()
{
	return bHasJustLanded;
}

bool APlayerCharacterBase::IsPunching()
{
	return bPunching;
}


void APlayerCharacterBase::NotifyCompletedAnimation(UAnimSequenceBase* CompletedAnimation)
{
	const auto AnimationName = CompletedAnimation->GetName();

	UE_LOG(LogTemp, Warning, TEXT("Animation name: %s"), *AnimationName);
	
	if (AnimationName.Equals("Landing"))
	{
		bHasJustLanded = false;
	}
	if (AnimationName.Equals("Punch"))
	{
		bPunching = false;
	}
}

void APlayerCharacterBase::NotifyAnimationEvent(FString EventData)
{
	if (EventData.Equals("Footstep"))
	{
		// Get sound
		USoundBase* SoundToPlay = bIsRightFoot ? RightFootstep : LeftFootstep;

		if (SoundToPlay)
		{
			// Get foot location
			const FVector FootLocation = bIsRightFoot ?
				GetMesh()->GetSocketLocation(FName(TEXT("RightFoot"))) :
				GetMesh()->GetSocketLocation(FName(TEXT("LeftFoot")));

			LastPlayedSound = UGameplayStatics::SpawnSoundAtLocation(
				GetWorld(),
				SoundToPlay,
				FootLocation,
				FRotator::ZeroRotator,
				2.0f
			);
		}
	}
	else if (EventData.Equals("Flip Finished"))
	{
		bSecondJump = false;
	}
	
}

bool APlayerCharacterBase::IsSecondJump()
{
	return bSecondJump;
}

#pragma region WALL RUNNING

bool APlayerCharacterBase::IsWallRunning()
{
	return bIsWallRunning;
}

EWallRunSide APlayerCharacterBase::GetWallRunningSide()
{
	return WallRunSide;
}

void APlayerCharacterBase::MovingForward()
{
}

bool APlayerCharacterBase::ConsumeJump()
{
	// If we are wall running
	if (bIsWallRunning)
	{
		// Always want to be able to jump off. Doesn't consume a jump because it doesn't need to
		return true;
	}

	// If we have jumps left
	if (JumpsLeft > 0)
	{
		// Remove a jump
		JumpsLeft--;

		// Consumed a jump
		return true;
		
	}

	return false;
	
}

void APlayerCharacterBase::CalculateRunDirectionAndSide(FVector WallNormal)
{
	// Get 2D planes 
	const auto WallNormal2D = FVector2D(WallNormal);
	const auto PlayerRight2D = FVector2D(GetActorRightVector());

	// Calculate dot product
	const float WallPlayerDot = FVector2D::DotProduct(WallNormal2D, PlayerRight2D);

	// Calculate and store variables
	if (WallPlayerDot > 0.0f)
	{
		WallRunSide = EWallRunSide::Right;
		WallRunDirection = FVector::CrossProduct(WallNormal, FVector(0.0f, 0.0f, 1.0f));
	}
	else
	{
		WallRunSide = EWallRunSide::Left;
		WallRunDirection = FVector::CrossProduct(WallNormal, FVector(0.0f, 0.0f, -1.0f));
	}

	
}

bool APlayerCharacterBase::CanSurfaceBeWallRan(FVector SurfaceNormal)
{
	if (SurfaceNormal.Z < -0.05)
	{
		return false;
	}

	FVector WallNormal = FVector(SurfaceNormal.X, SurfaceNormal.Y, 0.0f);
	WallNormal.Normalize();

	// This is the slope of the wall
	const auto Slope = FMath::DegreesToRadians(FVector::DotProduct(WallNormal, SurfaceNormal));

	const auto SlopeAngle = FMath::Acos(Slope);

	const float WalkableFloorAngle = GetCharacterMovement()->GetWalkableFloorAngle();

	if (SlopeAngle < WalkableFloorAngle)
	{
		return true;
	}

	return false;
}

FVector APlayerCharacterBase::FindLaunchVelocity()
{
	// Store our return value
	FVector LaunchDirection;

	// Check if we are wall running
	if (bIsWallRunning)
	{	
		switch (WallRunSide)
		{
		case EWallRunSide::Left:
		{
			LaunchDirection = FVector::CrossProduct(WallRunDirection, FVector(0.0f, 0.0f, 1.0f));
			break;
		}
		case EWallRunSide::Right:
		{
			LaunchDirection = FVector::CrossProduct(WallRunDirection, FVector(0.0f, 0.0f, -1.0f));
			break;
		}
		}
	}
	else
	{
		// Get the movement component
		const auto CharacterMovementRef = GetCharacterMovement();

		// If we are falling we need to calculate correct direction from button presses
		if (CharacterMovementRef->IsFalling())
		{
			// Calculate vectors
			const FVector RightVector = GetActorRightVector() * RightAxisValue;
			const FVector ForwardVector = GetActorForwardVector() * ForwardAxisValue;

			// Set launch direction to that direction
			LaunchDirection = RightVector + ForwardVector;
		}
		
	}

	// Add our up direction because regardless of previous calcs we always want to go up somewhat
	LaunchDirection += FVector(0.0f, 0.0f, 1.0f);

	// Multiply by char jump velocity
	LaunchDirection *= GetCharacterMovement()->JumpZVelocity;

	return LaunchDirection;
}

bool APlayerCharacterBase::AreRequiredKeysDown()
{
	// We must be holding positive forward axis
	if (!(ForwardAxisValue > 0.1f))
	{
		return false;
	}

	// Based on side check right axis value
	switch (WallRunSide)
	{
	case EWallRunSide::Left:
		return RightAxisValue > 0.1f;
	case EWallRunSide::Right: 
		return RightAxisValue < -0.1f;
	default: 
		return false;
	}
	
}

void APlayerCharacterBase::SetHorizontalVelocity(FVector2D newHorizontalVelocity)
{
	// Get movement
	const auto CharacterMovementRef = GetCharacterMovement();

	// Get current velocity
	FVector velocity = CharacterMovementRef->Velocity;

	// Set to new values
	velocity.X = newHorizontalVelocity.X;
	velocity.Y = newHorizontalVelocity.Y;

	// Set back 
	CharacterMovementRef->Velocity = velocity;
}

void APlayerCharacterBase::ClampHorizontalVelocity()
{
	const auto CharacterMovementRef = GetCharacterMovement();

	// Don't care for clamping unless we are in the air
	if (!CharacterMovementRef->IsFalling())
	{
		return;
	}

	// Get the current horizontal velocity
	const auto CurrHorizontalVelocity = FVector2D(CharacterMovementRef->Velocity);

	// Extract length
	FVector2D NormalisedHorizontalVelocity;
	float VelocityLength;
	CurrHorizontalVelocity.ToDirectionAndLength(NormalisedHorizontalVelocity, VelocityLength);

	// Divide and check
	const float DividedLength = VelocityLength / CharacterMovementRef->GetMaxSpeed();

	// If we are over the speed
	if (DividedLength > 1.0f)
	{
		SetHorizontalVelocity(CurrHorizontalVelocity / DividedLength);
	}
}

void APlayerCharacterBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	// When we land reset our jumps
	ResetJump(MaxJumps);

	// Back to 0 air control
	GetCharacterMovement()->AirControl = 0.0f;

	bHasJustLanded = true;
}

void APlayerCharacterBase::ResetJump(int ResetToJumps)
{
	JumpsLeft = FMath::Clamp<int>(ResetToJumps, 0, MaxJumps);
}

void APlayerCharacterBase::BeginWallRun()
{
	// Get movement comp
	const auto CharacterMovementRef = GetCharacterMovement();
	
	// Set air control to be 1 so we have actual control (unreal thinks we are in the air)
	CharacterMovementRef->AirControl = 1.0f;

	// Disable gravity
	CharacterMovementRef->GravityScale = 0.0f;

	// Constraint plane normal to XY
	CharacterMovementRef->SetPlaneConstraintNormal(FVector(0.0f, 0.0f, 1.0f));

	// Set wall running
	bIsWallRunning = true;
}

void APlayerCharacterBase::UpdateWallRun()
{
	// Check world is still valid (no reason it shouldnt ever be)
	const auto World = GetWorld();

	if (World)
	{
		const auto CharacterMovementRef = GetCharacterMovement();
		
		// If the keys are let go
		if (!AreRequiredKeysDown())
		{
			// Then fall off the wall
			EndWallRun(EWallRunEndReason::FallOff);
			return;
		}

		// Check for the wall
		// Start of trace is our locaiton
		const auto& Start = GetActorLocation();

		// End of trace is perpendicular to our running direction
		auto End = Start;
		switch (WallRunSide)
		{
		case EWallRunSide::Left:
			End += FVector::CrossProduct(WallRunDirection, FVector(0.0f, 0.0f, -200.0f));
			break;
		case EWallRunSide::Right:
			End += FVector::CrossProduct(WallRunDirection, FVector(0.0f, 0.0f, 200.0f));
			break;
		}

		// Check if we hit something to our side
		FHitResult OutHit;
		if (!World->LineTraceSingleByChannel(OutHit,Start,End,ECC_Visibility))
		{
			// If we didnt we fell off
			EndWallRun(EWallRunEndReason::FallOff);
			return;
		}

		// Work out which direction we should keep running in to account for curves

		// Store current side before we do this. This wall make you fall off if you rotate camera away from direction
		const EWallRunSide CurrentSide = WallRunSide;

		// Update direction and side
		CalculateRunDirectionAndSide(OutHit.ImpactNormal);

		// Side has changed so we must have span camera
		if (CurrentSide != WallRunSide)
		{
			EndWallRun(EWallRunEndReason::FallOff);
			return;
		}

		// Update velocity with new direction
		CharacterMovementRef->Velocity = FVector(WallRunDirection.X, WallRunDirection.Y, 0.0f) * CharacterMovementRef->MaxWalkSpeed;
		

	}


	
}

void APlayerCharacterBase::EndWallRun(EWallRunEndReason Reason)
{
	switch (Reason)
	{
	case EWallRunEndReason::FallOff:
		ResetJump(1);
		break;
	case EWallRunEndReason::JumpOff:
		ResetJump(MaxJumps - 1);
		break;
	}

	// Get movement comp
	const auto CharacterMovementRef = GetCharacterMovement();

	// Reset air control to 0.3. We want some control after we launch off 
	CharacterMovementRef->AirControl = 0.3f;

	// Enable gravity
	CharacterMovementRef->GravityScale = 1.0f;

	// Remove contstiants
	CharacterMovementRef->SetPlaneConstraintNormal(FVector(0.0f, 0.0f,0.0f));

	// Disable wall running
	bIsWallRunning = false;
}

void APlayerCharacterBase::CheckWallOnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{	
	const auto CharacterMovementRef = GetCharacterMovement();
	
	// If we are already on a wall we dont want to do anything
	if (!bIsWallRunning)
	{
		// Check if we can run on the thing we have hit
		if (CanSurfaceBeWallRan(Hit.ImpactNormal))
		{
			// We need to be falling (i.e. we have just jumped)
			if (CharacterMovementRef->IsFalling())
			{
				// Calculate the side and direction
				CalculateRunDirectionAndSide(Hit.ImpactNormal);

				// Finally check the keys are correct
				if (AreRequiredKeysDown())
				{
					// We can begin a wall run
					BeginWallRun();
				}
			}
		}
	}
	
}

float APlayerCharacterBase::IKFootTrace(const FName& Socket)
{
	const auto& SocketLocation = GetMesh()->GetSocketLocation(Socket);
	const auto& PlayerLocation = GetActorLocation();

	const auto Start = FVector(SocketLocation.X, SocketLocation.Y, PlayerLocation.Z);
	const auto End = FVector(SocketLocation.X, SocketLocation.Y, PlayerLocation.Z + IKTraceDistance);

	const auto& World = GetWorld();

	FHitResult IKHit;
	if (World->LineTraceSingleByChannel(IKHit,Start,End,ECC_Visibility))
	{
		// Calc the IK offset
		const FVector IKVector = End - IKHit.Location;

		// Deconstruct the length
		FVector IKDirection;
		float IKLength;
		IKVector.ToDirectionAndLength(IKDirection, IKLength);

		return IKLength * IKScale;		
		
	}
	return 0.0f;
}

void APlayerCharacterBase::UpdateIKVariables(float DeltaTime)
{
	// Update the right foot
	IKRightFoot = FMath::FInterpTo(IKRightFoot, IKFootTrace(IKRightFootSocket), DeltaTime, IKInteropSpeed);

	//update the left foot
	IKLeftFoot = FMath::FInterpTo(IKLeftFoot, IKFootTrace(IKLeftFootSocket), DeltaTime, IKInteropSpeed);
}


float APlayerCharacterBase::GetRightFootIK()
{
	return IKRightFoot;
}

float APlayerCharacterBase::GetLeftFootIK()
{
	return IKLeftFoot;
}

void APlayerCharacterBase::FellOutOfWorld(const UDamageType& dmgType)
{
	// Kills player
	Super::FellOutOfWorld(dmgType);

	// Get world ref
	const auto World = GetWorld();

	if (World)
	{
		const auto AuthGameMode = Cast<AGrappleGameModeBase>(World->GetAuthGameMode());

		if (AuthGameMode)
		{
			// Check for active check points
			TArray<AActor*> CheckpointList;
			UGameplayStatics::GetAllActorsOfClass(World, ACheckpointActorBase::StaticClass(), CheckpointList);

			if (CheckpointList.Num() != 0)
			{
				// Cast to correct type
				// This is a formality. Because of GetAllActors taking a static class input, i KNOW these pointers are this type
				// Therefore down casting is safe
				TArray<ACheckpointActorBase*> CheckpointsCast;
				for (int32 i = 0; i < CheckpointList.Num(); ++i)
				{
					CheckpointsCast.Add(Cast<ACheckpointActorBase>(CheckpointList[i]));
				}

				// Now sort by level index to put the end at the front
				CheckpointsCast.Sort([&](const ACheckpointActorBase& lhs, const ACheckpointActorBase& rhs)
				{
					return lhs.CheckpointIndex > rhs.CheckpointIndex;
				});

				// Find the latest activated checkpoint
				const ACheckpointActorBase* SelectedCheckpoint = nullptr;
				for (int32 i = 0; i < CheckpointsCast.Num(); ++i)
				{
					if (CheckpointsCast[i]->bIsChecked)
					{
						SelectedCheckpoint = CheckpointsCast[i];
						break;
					}
				}

				// Restart player and immediately teleport to checkpoint
				AuthGameMode->RestartPlayer(World->GetFirstPlayerController());
				const auto Player = UGameplayStatics::GetPlayerCharacter(World, 0);
				if (Player && SelectedCheckpoint)
				{
					Player->SetActorLocation(SelectedCheckpoint->GetActorLocation());
				}
				
				
			}
			else
			{
				// If no checkpoints then simply reload at player start
				AuthGameMode->RestartPlayer(World->GetFirstPlayerController());
			}
		}
	}
}

#pragma endregion

