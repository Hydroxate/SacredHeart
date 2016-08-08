// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "SacredHeart.h"
#include "SacredHeartCharacter.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/InputSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ASacredHeartCharacter

ASacredHeartCharacter::ASacredHeartCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	PrimaryActorTick.bCanEverTick = true;

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	WalkSpeed = 250;
	SprintSpeed = 600;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachParent = GetCapsuleComponent();
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->AttachParent = FirstPersonCameraComponent;
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	
	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P are set in the
	// derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void ASacredHeartCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Ensure the player starts with Walking
	stopSprinting();

}

void ASacredHeartCharacter::Tick(float DeltaSeconds)
{

	if (GetWorldTimerManager().GetTimerElapsed(HeartCalibrationTimerHandle) == 60.0f)
	{
		averageHeartRate = averageHeartRate / 60;
		GetWorldTimerManager().ClearTimer(HeartCalibrationTimerHandle);
		
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASacredHeartCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAction("Sprint", IE_Pressed, this, &ASacredHeartCharacter::startSprinting);
	InputComponent->BindAction("Sprint", IE_Released, this, &ASacredHeartCharacter::stopSprinting);

	

	InputComponent->BindAxis("MoveForward", this, &ASacredHeartCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ASacredHeartCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ASacredHeartCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ASacredHeartCharacter::LookUpAtRate);
}


void ASacredHeartCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void ASacredHeartCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{

	}
	TouchItem.bIsPressed = false;
}

void ASacredHeartCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
	{
		if (TouchItem.bIsPressed)
		{
			if (GetWorld() != nullptr)
			{
				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
				if (ViewportClient != nullptr)
				{
					FVector MoveDelta = Location - TouchItem.Location;
					FVector2D ScreenSize;
					ViewportClient->GetViewportSize(ScreenSize);
					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.X * BaseTurnRate;
						AddControllerYawInput(Value);
					}
					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.Y * BaseTurnRate;
						AddControllerPitchInput(Value);
					}
					TouchItem.Location = Location;
				}
				TouchItem.Location = Location;
			}
		}
	}
}

void ASacredHeartCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ASacredHeartCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ASacredHeartCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASacredHeartCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool ASacredHeartCharacter::EnableTouchscreenMovement(class UInputComponent* InputComponent)
{
	bool bResult = false;
	if (FPlatformMisc::GetUseVirtualJoysticks() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		bResult = true;
		InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ASacredHeartCharacter::BeginTouch);
		InputComponent->BindTouch(EInputEvent::IE_Released, this, &ASacredHeartCharacter::EndTouch);
		InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ASacredHeartCharacter::TouchUpdate);
	}
	return bResult;
}

void ASacredHeartCharacter::getAverageHeartRate()
{
	//for (int i = 0; i < 60; i++)
	//{
	GetWorldTimerManager().SetTimer(HeartCalibrationTimerHandle, this, &ASacredHeartCharacter::sumAverageHeart, 1.0f, true);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, "TimerManager");
	//}

}

void ASacredHeartCharacter::sumAverageHeart()
{
	averageHeartRate += heartRate;
}

void ASacredHeartCharacter::startSprinting()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, "Sprinting");
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

}

void ASacredHeartCharacter::stopSprinting()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, "Walking");
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

}