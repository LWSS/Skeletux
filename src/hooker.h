#pragma once

#include <memory>
#include <unordered_map>
#include <sys/mman.h>
#include <link.h>
#include "Utils/patternfinder.h"
#include "Utils/recvproxyhook.h"
#include "SDK/SDK.h"
#include "Utils/vmt.h"
#include "Utils/util.h"
#include "interfaces.h"

struct dlinfo_t
{
	const char* library = nullptr;
	uintptr_t address = 0;
	size_t size = 0;
};

namespace Hooker
{
	bool GetLibraryInformation(const char* library, uintptr_t* address, size_t* size);
	void InitializeVMHooks();
	bool HookRecvProp(const char* className, const char* propertyName, std::unique_ptr<RecvPropHook>& recvPropHook);
	void FindIClientMode();
	void FindGlobalVars();
	void FindCInput();
	void FindPlayerResource();
	void FindGameRules();
	void FindViewRender();
	void FindPrediction();
	void FindSurfaceDrawing();
	void FindGetLocalClient();
	void FindInitKeyValues();
	void FindLoadFromBuffer();
	void FindOverridePostProcessingDisable();
	void FindSDLInput();
	void FindSetNamedSkybox();
	void FindPanelArrayOffset();
}
