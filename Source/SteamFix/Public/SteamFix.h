// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSteamFix, Log, All);

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
	void SteamButtonClicked();

private:

	FString GetEditorExe(IFileManager& FileManager);
	FString GetProjectFile(IFileManager& FileManager);
	FString GetCmdParams(FString UE4EditorExe, FString ProjectFile);

	void RegisterMenus();
	void AddToolbarExtension(FToolBarBuilder& Builder);

	TSharedPtr<class FUICommandList> PluginCommands;
};
