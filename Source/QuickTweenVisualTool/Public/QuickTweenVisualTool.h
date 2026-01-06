// Copyright 2025 Juan Pablo Hernandez Mosti. All Rights Reserved.
#pragma once

#include "Modules/ModuleManager.h"

class FQuickTweenVisualToolModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

	void RegisterMenus();
	void OpenQuickTweenVisualToolWindow();
};