#include "hooks.h"
#include "../Hacks/DisablePostProcessing.h"

void Hooks::BeginFrame(void* thisptr, float frameTime)
{
	DisablePostProcessing::BeginFrame();

	return materialVMT->GetOriginalMethod<BeginFrameFn>(42)(thisptr, frameTime);
}
