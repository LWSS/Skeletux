#include "hooks.h"

extern StartDrawingFn StartDrawing;
extern FinishDrawingFn FinishDrawing;

void Hooks::Paint(void* thisptr, PaintMode_t mode)
{
	engineVGuiVMT->GetOriginalMethod<PaintFn>(15)(thisptr, mode);

	if (mode & PAINT_UIPANELS)
	{
		StartDrawing(surface);

			// SURFACE DRAWING HERE

		FinishDrawing(surface);
	}
}
