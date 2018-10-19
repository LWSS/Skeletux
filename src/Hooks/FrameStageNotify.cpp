#include "hooks.h"
#include "../Hacks/SkyBox.h"

void Hooks::FrameStageNotify(void* thisptr, ClientFrameStage_t stage)
{
	SkyBox::FrameStageNotify( stage );

	clientVMT->GetOriginalMethod<FrameStageNotifyFn>(37)(thisptr, stage);
}
