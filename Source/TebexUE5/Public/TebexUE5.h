#pragma once

#include "CoreMinimal.h"
#include "Headless/EmptyBody.h"
#include "Modules/ModuleManager.h"

class FTebexUE5 : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	static void Log(const FString& Message);

	const static FEmptyBody EmptyRequest;
};
