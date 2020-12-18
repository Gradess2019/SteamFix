// Copyright Epic Games, Inc. All Rights Reserved.

#include "SteamFixCommands.h"

#define LOCTEXT_NAMESPACE "FSteamFixModule"

void FSteamFixCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "SteamFix", "Execute SteamFix action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
