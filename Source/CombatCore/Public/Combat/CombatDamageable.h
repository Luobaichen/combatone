#pragma once

#include"CoreMinimal.h"
#include"UObject/Interface.h"
#include"Combat/CombatTypes.h"
#include"CombatDamageable.generated.h"

UINTERFACE(MinimalAPI,Blueprintable)
class UCombatDamageable : public UInterface
{
	GENERATED_BODY()
};

class COMBATCORE_API ICombatDamageable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void ApplyHit(const FCombatHitInfo& HitInfo);
};





