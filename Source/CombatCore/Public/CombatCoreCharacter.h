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
class ACombatWeapon;
class UMeleeTraceComponent;
class UCombatAttributeComponent;
class UAnimMontage;


UCLASS()
class COMBATCORE_API ACombatCoreCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACombatCoreCharacter();

	UFUNCTION(Exec)
	void DebugHitDummy(float Damage);

	UFUNCTION(BlueprintPure, Category = "Combat|Weapon")
	ACombatWeapon* GetWeapon() const;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UMeleeTraceComponent> MeleeTrace;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UCombatAttributeComponent> AttributeComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TSubclassOf<ACombatWeapon>WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Attack")
	TObjectPtr<UAnimMontage> LightAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Attack")
	TObjectPtr<UAnimMontage> HeavyAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Attack", meta = (ClampMin = "0.0"))
	float LightAttackStaminaCost = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Attack", meta = (ClampMin = "0.0"))
	float HeavyAttackStaminaCost = 25.f;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName HandSocketName = TEXT("HandGrip_R");

	UPROPERTY()
	TObjectPtr<ACombatWeapon> Weapon;

	void LightAttack();
	void HeavyAttack();
	void TryPlayAttack(UAnimMontage* Montage, float StaminaCost);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MouseLookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> HeavyAttackAction;

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
