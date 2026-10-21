// Copyright Epic Games, Inc. All Rights Reserved.

#include "CombatCoreGameMode.h"

#include "CombatCoreCharacter.h"

ACombatCoreGameMode::ACombatCoreGameMode()
{
	// Tell the game mode which pawn to spawn for each player.
	DefaultPawnClass = ACombatCoreCharacter::StaticClass();
}
