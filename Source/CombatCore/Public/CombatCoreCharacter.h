// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatCoreCharacter.generated.h"

// Forward declarations: the header only needs to know these types exist.
// The full definitions are included in the .cpp, which keeps compile times short.
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

/**
 * Player character for the CombatCore prototype.
 *
 * Day 1 scope: third-person camera and Enhanced Input movement/look.
 * Combat components are added in the following days.
 */
UCLASS()
class COMBATCORE_API ACombatCoreCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACombatCoreCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/** Camera boom: keeps the camera at a fixed distance behind the character. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	/** Camera placed at the end of the boom. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;

	/** WASD / arrow keys / gamepad sticks. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/** Mouse delta. The engine template keeps this in a separate context. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> MouseLookMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MouseLookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	/** Ground movement speed in cm/s. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float MoveSpeed = 500.f;

	/** Multiplier applied to look input. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float LookSensitivity = 1.f;

	/** 2D movement input: X = right, Y = forward. */
	void Move(const FInputActionValue& Value);

	/** 2D look input: X = yaw, Y = pitch. */
	void Look(const FInputActionValue& Value);
};
