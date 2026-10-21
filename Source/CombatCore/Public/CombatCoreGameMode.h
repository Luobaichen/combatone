// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CombatCoreGameMode.generated.h"

/**
 * Default game mode for CombatCore. Spawns the C++ player character and
 * (later) the HUD.
 */
UCLASS()
class COMBATCORE_API ACombatCoreGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACombatCoreGameMode();
};
