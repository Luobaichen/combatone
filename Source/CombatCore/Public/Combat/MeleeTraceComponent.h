// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeleeTraceComponent.generated.h"

class ACombatWeapon;


	UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
	class COMBATCORE_API UMeleeTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMeleeTraceComponent();

	UFUNCTION(BlueprintCallable, Category = "Combat|Trace")
	void StartTrace(ACombatWeapon* InWeapon, float InDamage, float InRadius);

	UFUNCTION(BlueprintCallable, Category = "Combat|Trace")
	void StopTrace();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Trace")
	bool bDrawDebug = true;

protected:
	// Called when the game starts
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void PerformSweep();

	UPROPERTY()
	TObjectPtr<ACombatWeapon> Weapon;

	float Damage = 0.f;
	float Radius = 15.f;

	bool bTracing = false;

	FVector PreviousStart = FVector::ZeroVector;
	FVector PreviousEnd = FVector::ZeroVector;

	TSet<TWeakObjectPtr<AActor>> HitActors;

};
