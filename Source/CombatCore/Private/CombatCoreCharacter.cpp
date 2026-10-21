// Copyright Epic Games, Inc. All Rights Reserved.

#include "CombatCoreCharacter.h"

#include "CombatCore.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Engine/SkeletalMesh.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "UObject/ConstructorHelpers.h"

ACombatCoreCharacter::ACombatCoreCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// Capsule size matches the UE5 mannequin (radius 42, half height 96).
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	// The controller rotates the camera, not the character. If these were true
	// the whole character would spin with the mouse.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Movement defaults: character turns towards its velocity instead of strafing.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;

	// Camera boom. bUsePawnControlRotation makes it follow the controller (mouse).
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.f;
	SpringArm->bUsePawnControlRotation = true;

	// Camera at the end of the boom. It must NOT apply control rotation again,
	// because the boom already did.
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	// Skeletal mesh from the engine template. The mesh is offset so its feet
	// line up with the bottom of the capsule, and rotated to face +X.
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -96.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(
		TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple"));
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	}

	// FClassFinder takes the Blueprint asset path without the _C suffix.
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimClass(
		TEXT("/Game/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed"));
	if (AnimClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimClass.Class);
	}

	// Input assets. FObjectFinder paths end in .AssetName.
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultContextAsset(
		TEXT("/Game/Input/IMC_Default.IMC_Default"));
	if (DefaultContextAsset.Succeeded())
	{
		DefaultMappingContext = DefaultContextAsset.Object;
	}

	// Mouse look lives in its own context in the engine template.
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> MouseLookContextAsset(
		TEXT("/Game/Input/IMC_MouseLook.IMC_MouseLook"));
	if (MouseLookContextAsset.Succeeded())
	{
		MouseLookMappingContext = MouseLookContextAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveAsset(
		TEXT("/Game/Input/Actions/IA_Move.IA_Move"));
	if (MoveAsset.Succeeded())
	{
		MoveAction = MoveAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LookAsset(
		TEXT("/Game/Input/Actions/IA_Look.IA_Look"));
	if (LookAsset.Succeeded())
	{
		LookAction = LookAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MouseLookAsset(
		TEXT("/Game/Input/Actions/IA_MouseLook.IA_MouseLook"));
	if (MouseLookAsset.Succeeded())
	{
		MouseLookAction = MouseLookAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpAsset(
		TEXT("/Game/Input/Actions/IA_Jump.IA_Jump"));
	if (JumpAsset.Succeeded())
	{
		JumpAction = JumpAsset.Object;
	}
}

void ACombatCoreCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Keep the movement component in sync if MoveSpeed is changed in a
	// Blueprint subclass.
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

	// Enhanced Input contexts are per local player, so they are added here
	// (the controller exists by BeginPlay) instead of in the constructor.
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
			else
			{
				UE_LOG(LogCombatCore, Warning, TEXT("DefaultMappingContext is not set on %s"), *GetName());
			}

			if (MouseLookMappingContext)
			{
				Subsystem->AddMappingContext(MouseLookMappingContext, 1);
			}
			else
			{
				UE_LOG(LogCombatCore, Warning, TEXT("MouseLookMappingContext is not set on %s"), *GetName());
			}
		}
	}
}

void ACombatCoreCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// The project uses EnhancedInputComponent (see Config/DefaultInput.ini).
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACombatCoreCharacter::Move);
		}

		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACombatCoreCharacter::Look);
		}

		if (MouseLookAction)
		{
			EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ACombatCoreCharacter::Look);
		}

		if (JumpAction)
		{
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		}

		if (!MoveAction || !LookAction)
		{
			UE_LOG(LogCombatCore, Warning, TEXT("One or more input actions are missing on %s"), *GetName());
		}
	}
	else
	{
		UE_LOG(LogCombatCore, Error,
			TEXT("'%s' failed to find an Enhanced Input component. This project uses Enhanced Input."),
			*GetNameSafe(this));
	}
}

void ACombatCoreCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller && !MovementVector.IsNearlyZero())
	{
		// Use only the yaw of the control rotation, so looking up/down does not
		// slow the character down or make it fly.
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ACombatCoreCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	// Mouse/gamepad input is already scaled by the input asset; this is an
	// extra user-facing sensitivity multiplier.
	AddControllerYawInput(LookAxisVector.X * LookSensitivity);
	AddControllerPitchInput(LookAxisVector.Y * LookSensitivity);
}
