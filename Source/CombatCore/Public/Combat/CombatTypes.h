#pragma once

#include "CoreMinimal.h"
#include "CombatTypes.generated.h"

class AActor;

USTRUCT(BlueprintType)
struct COMBATCORE_API FCombatHitInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	float Damage = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	FVector HitLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	FVector HitNormal = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> Instigator = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	FName HitBoneName = NAME_None;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	bool bHeavyAttack = false;
};
