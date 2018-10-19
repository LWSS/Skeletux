#pragma once

#include <unordered_map>
#include <zconf.h>
#include <fstream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include "json/json.h"
#include "SDK/SDK.h"
#include "Utils/draw.h"
#include "Utils/util.h"
#include "Utils/util_sdk.h"

namespace Settings
{

	namespace SkyBox
	{
		inline bool enabled;
		inline const char* name = NULL;
	}

	namespace DisablePostProcessing
	{
		inline bool enabled;
	}

	namespace NoFog
	{
		inline bool enabled;
	}
}
