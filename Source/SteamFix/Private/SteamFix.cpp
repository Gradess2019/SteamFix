// Copyright Epic Games, Inc. All Rights Reserved.

#include "SteamFix.h"
#include "SteamFixStyle.h"
#include "SteamFixCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

static const FName SteamFixTabName("SteamFix");

#define LOCTEXT_NAMESPACE "FSteamFixModule"

void FSteamFixModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FSteamFixStyle::Initialize();
	FSteamFixStyle::ReloadTextures();

	FSteamFixCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FSteamFixCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FSteamFixModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FSteamFixModule::RegisterMenus));
}

void FSteamFixModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FSteamFixStyle::Shutdown();

	FSteamFixCommands::Unregister();
}

void FSteamFixModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FSteamFixModule::PluginButtonClicked()")),
							FText::FromString(TEXT("SteamFix.cpp"))
					   );
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FSteamFixModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Game");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FSteamFixCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSteamFixModule, SteamFix)