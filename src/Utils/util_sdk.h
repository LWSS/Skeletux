#pragma once

#include <vector>
#include "../SDK/SDK.h"
#include "../interfaces.h"
#include "util.h"

namespace Util
{
	inline std::vector<ConVar *> createdConvars = {};

	bool RegisterConVar( const char *name, const char *value, uint32_t flags = 136, const char *helpString = NULL, bool bMin = false, float fMin = 0.0f, bool bMax = false, float fMax = 1.0f );
}
