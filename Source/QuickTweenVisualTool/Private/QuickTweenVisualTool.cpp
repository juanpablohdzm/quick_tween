// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.
#include "QuickTweenVisualTool.h"
#include "CoreMinimal.h"
#include "SQuickTweenEditor.h"
#include "Modules/ModuleManager.h"

static const FName QuickTweenEditorTabName("QuickTweenVisualTool");

void FQuickTweenVisualToolModule::StartupModule()
{
	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FQuickTweenVisualToolModule::RegisterMenus)
	);

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		QuickTweenEditorTabName,
		FOnSpawnTab::CreateLambda([](const FSpawnTabArgs&)
		{
			return SNew(SDockTab)
				.TabRole(NomadTab)
				[
					SNew(SQuickTweenEditor)
				];
		})
	)
	.SetDisplayName(FText::FromString("Quick Tween Editor"))
	.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FQuickTweenVisualToolModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(QuickTweenEditorTabName);
}

void FQuickTweenVisualToolModule::RegisterMenus()
{
	UToolMenu* menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");

	FToolMenuSection& section = menu->FindOrAddSection("WindowLayout");
	section.AddMenuEntry(
		"QuickTweenEditor",
		FText::FromString("Quick Tween Visual Tool"),
		FText::FromString("Open the Quick Tween Visual Tool window"),
		FSlateIcon(),
		FUIAction(
			FExecuteAction::CreateRaw(this, &FQuickTweenVisualToolModule::OpenQuickTweenVisualToolWindow)
		)
	);
}

void FQuickTweenVisualToolModule::OpenQuickTweenVisualToolWindow()
{
	FGlobalTabmanager::Get()->TryInvokeTab(QuickTweenEditorTabName);
}

IMPLEMENT_MODULE(FQuickTweenVisualToolModule, QuickTweenVisualTool)
