#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeleeTraceComponent.generated.h"

class ACombatWeapon;

/** Sweeps a sphere between the weapon's previous and current trace points. */
UCLASS(ClassGroup = (Combat), meta = (BlueprintSpawnableComponent))
class COMBATCORE_API UMeleeTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMeleeTraceComponent();

	UFUNCTION(BlueprintCallable, Category = "Combat|Trace")
	void StartTrace(ACombatWeapon* InWeapon, float InDamage, float InRadius, bool bInHeavyAttack = false);

	UFUNCTION(BlueprintCallable, Category = "Combat|Trace")
	void StopTrace();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Trace")
	bool bDrawDebug = false;

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void PerformSweep();

	UPROPERTY()
	TObjectPtr<ACombatWeapon> Weapon;

	float Damage = 0.f;
	float Radius = 15.f;

	bool bTracing = false;
	bool bHeavyAttack = false;

	FVector PreviousStart = FVector::ZeroVector;
	FVector PreviousEnd = FVector::ZeroVector;

	TSet<TWeakObjectPtr<AActor>> HitActors;

};
