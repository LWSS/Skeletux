#pragma once

#include "../settings.h"

namespace GUI
{
	/* CSGO Panels */
	inline panorama::IUIPanel* menuRoot;
	inline panorama::IUIPanel* hudRoot;
	/* Custom Panels */
	inline panorama::IUIPanel* skeleMain;

	void ToggleUI();
}