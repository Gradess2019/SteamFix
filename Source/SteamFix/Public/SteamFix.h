// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FSteamFixModule : public IModuleInterface
{
public:

	void AddButtonToBlueprintEditor();
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();
	
private:

	void RegisterMenus();
	void AddToolbarExtension(FToolBarBuilder& Builder);

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};
