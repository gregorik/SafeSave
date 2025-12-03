// Copyright Epic Games, Inc. All Rights Reserved.

#include "SafeSaveModule.h"
#include "SafeSaveStyle.h"
#include "SSafeSaveToolbar.h"

// Engine Framework
#include "Editor.h"
#include "ToolMenus.h"
#include "Subsystems/AssetEditorSubsystem.h"

// UI / Notifications
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"

// Source Control
#include "ISourceControlModule.h"
#include "ISourceControlProvider.h"
#include "SourceControlOperations.h"

#define LOCTEXT_NAMESPACE "FSafeSaveModule"

void FSafeSaveModule::StartupModule()
{
	FSafeSaveStyle::Initialize();
	FSafeSaveStyle::ReloadTextures();

	if (UToolMenus::Get())
	{
		UToolMenus::RegisterStartupCallback(
			FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FSafeSaveModule::RegisterMenus)
		);
	}

	if (GEditor)
	{
		if (UAssetEditorSubsystem* AssetSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
		{
			AssetSubsystem->OnAssetEditorOpened().AddRaw(this, &FSafeSaveModule::OnAssetOpened);
		}
	}
}

void FSafeSaveModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);

	if (GEditor)
	{
		if (UAssetEditorSubsystem* AssetSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
		{
			AssetSubsystem->OnAssetEditorOpened().RemoveAll(this);
		}
	}

	FSafeSaveStyle::Shutdown();
}

void FSafeSaveModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);
	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.User");

	if (Menu)
	{
		FToolMenuSection& Section = Menu->FindOrAddSection("SafeSaveControls");
		FToolMenuEntry Entry = FToolMenuEntry::InitWidget(
			"SafeSaveStatusWidget",
			SNew(SSafeSaveToolbar),
			FText::GetEmpty(),
			true
		);
		Section.AddEntry(Entry);
	}
}

void FSafeSaveModule::OnAssetOpened(UObject* Asset)
{
	if (!Asset) return;

	ISourceControlModule& SCModule = ISourceControlModule::Get();
	if (!SCModule.IsEnabled()) return;

	ISourceControlProvider& Provider = SCModule.GetProvider();
	FSourceControlStatePtr State = Provider.GetState(Asset->GetPackage(), EStateCacheUsage::Use);

	// The "Billy" Logic
	if (State.IsValid() && State->IsCheckedOutOther())
	{
		// 1. Try to get the user info from the tooltip/display string
		FString WhoIsIt = State->GetDisplayTooltip().ToString();

		// 2. Fire the "Dropbox-Style" Toast Notification
		FNotificationInfo Info(FText::Format(LOCTEXT("LockedToast", "LOCKED by Teammate:\n{0}"), FText::FromString(WhoIsIt)));
		Info.ExpireDuration = 5.0f;
		Info.Image = FAppStyle::GetBrush("Icons.WarningWithColor");
		Info.bUseLargeFont = true;
		Info.ButtonDetails.Add(FNotificationButtonInfo(
			LOCTEXT("ReadOnly", "Make Read Only"),
			FText::GetEmpty(),
			FSimpleDelegate::CreateLambda([]() { /* Placeholder: Could force read-only mode */ })
		));

		FSlateNotificationManager::Get().AddNotification(Info);
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSafeSaveModule, SafeSave)