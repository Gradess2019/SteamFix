// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "SteamFixStyle.h"

class FSteamFixCommands : public TCommands<FSteamFixCommands>
{
public:

	FSteamFixCommands()
		: TCommands<FSteamFixCommands>(TEXT("SteamFix"), NSLOCTEXT("Contexts", "SteamFix", "SteamFix Plugin"), NAME_None, FSteamFixStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > ButtonAction;
};
