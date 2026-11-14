#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CombatAttributeTestListener.generated.h"

class AActor;

UCLASS()
class COMBATCORE_API UCombatAttributeTestListener : public UObject
{
    GENERATED_BODY()

public:
    int32 DeathCount = 0;

    UFUNCTION()
    void HandleDeath(AActor* Killer) { ++DeathCount; }
};
