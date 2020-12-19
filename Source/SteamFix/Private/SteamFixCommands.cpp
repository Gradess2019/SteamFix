// Copyright Epic Games, Inc. All Rights Reserved.

#include "SteamFixCommands.h"

#define LOCTEXT_NAMESPACE "FSteamFixModule"

void FSteamFixCommands::RegisterCommands()
{
	UI_COMMAND(ButtonAction, "Play with Steam", "Execute Standalone Mode with Steam", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
