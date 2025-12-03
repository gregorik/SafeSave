// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FSafeSaveModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	/** 
	 * Registers the "Traffic Light" widget into the main Level Editor Toolbar.
	 * Uses UToolMenus (modern replacement for FExtender).
	 */
	void RegisterMenus();

	/** 
	 * Callback triggered immediately when an asset editor (Blueprint, DataAsset, etc.) is opened.
	 * Checks lock status to prevent "Soft Locks".
	 */
	void OnAssetOpened(UObject* Asset);
};