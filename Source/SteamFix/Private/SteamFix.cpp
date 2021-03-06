// Copyright Epic Games, Inc. All Rights Reserved.

#include "SteamFix.h"

#include "BlueprintEditorModule.h"
#include "SteamFixStyle.h"
#include "SteamFixCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

DEFINE_LOG_CATEGORY(LogSteamFix);

static const FName SteamFixTabName("SteamFix");

#define LOCTEXT_NAMESPACE "FSteamFixModule"

void FSteamFixModule::AddButtonToBlueprintEditor()
{
	auto& BlueprintEditor = FModuleManager::LoadModuleChecked<FBlueprintEditorModule>("Kismet");
	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension(
		"Debugging",
		EExtensionHook::After,
		PluginCommands,
		FToolBarExtensionDelegate::CreateRaw(
			this,
			&FSteamFixModule::AddToolbarExtension
		)
	);
	BlueprintEditor.GetMenuExtensibilityManager()->AddExtender(ToolbarExtender);
}

void FSteamFixModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FSteamFixStyle::Initialize();
	FSteamFixStyle::ReloadTextures();

	FSteamFixCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FSteamFixCommands::Get().ButtonAction,
		FExecuteAction::CreateRaw(this, &FSteamFixModule::SteamButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FSteamFixModule::RegisterMenus));

	AddButtonToBlueprintEditor();
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

void FSteamFixModule::SteamButtonClicked()
{
	auto& FileManager = IFileManager::Get();
	const auto UE4EditorExe = GetEditorExe(FileManager);
	const auto ProjectFile = GetProjectFile(FileManager);
	const auto CmdPath = FString("C:\\Windows\\System32\\cmd.exe");
	const auto CmdParams = GetCmdParams(UE4EditorExe, ProjectFile);

	const auto bLaunchDetached = true;
	const auto bLaunchHidden = false;
	const auto bLaunchReallyHidden = false;

	auto OutProcessID = new uint32(-1);
	const auto PriorityModifier = 0;

	const auto OptionalWorkingDirectory = nullptr;
	const auto PipeWriteChild = nullptr;

	auto ProcHandle = FPlatformProcess::CreateProc(
		*CmdPath,
		*CmdParams,
		bLaunchDetached,
		bLaunchHidden,
		bLaunchReallyHidden,
		OutProcessID,
		PriorityModifier,
		OptionalWorkingDirectory,
		PipeWriteChild
	);

	UE_LOG(
		LogSteamFix,
		Log,
		TEXT("Cmd path: %s\n\tProject path: %s\n\tProcessID: %d\n\tCmd Params: %s"),
		*CmdPath,
		*ProjectFile,
		*OutProcessID,
		*CmdParams
	);
}

FString FSteamFixModule::GetEditorExe(IFileManager& FileManager)
{
	auto EngineDir = FPaths::EngineDir();
	auto UE4EditorExe = EngineDir.Append(TEXT("Binaries\\Win64\\UnrealEditor.exe"));
	FPaths::NormalizeFilename(UE4EditorExe);;
	UE4EditorExe = FileManager.ConvertToAbsolutePathForExternalAppForRead(*UE4EditorExe);

	return UE4EditorExe;
}

FString FSteamFixModule::GetProjectFile(IFileManager& FileManager)
{
	const auto ProjectDir = FPaths::ProjectDir();
	const auto Filter = FString(ProjectDir).Append(TEXT("*.uproject"));
	auto FileNames = TArray<FString>();
	FileManager.FindFiles(FileNames, *Filter, true, false);

	auto ProjectFile = FString(ProjectDir).Append(FileNames[0]);
	FPaths::NormalizeFilename(ProjectFile);;
	ProjectFile = FileManager.ConvertToAbsolutePathForExternalAppForRead(*ProjectFile);

	return ProjectFile;
}

FString FSteamFixModule::GetCmdParams(const FString UE4EditorExe, const FString ProjectFile)
{
	const auto MapName = GEditor->GetEditorWorldContext().World()->GetMapName();
	auto Args = FStringFormatOrderedArguments();
	Args.Add(UE4EditorExe);
	Args.Add(ProjectFile);
	Args.Add(MapName);

	return FString::Format(TEXT("cmd /c \"\"{0}\" \"{1}\"\" {2} -game -log"), Args);
}

void FSteamFixModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Play");
			{
				FToolMenuEntry& Entry = Section.AddEntry(
					FToolMenuEntry::InitToolBarButton(FSteamFixCommands::Get().ButtonAction)
				);
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

void FSteamFixModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FSteamFixCommands::Get().ButtonAction);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSteamFixModule, SteamFix)
