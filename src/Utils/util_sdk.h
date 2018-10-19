#pragma once

#include <vector>
#include "../SDK/SDK.h"
#include "../interfaces.h"
#include "util.h"

namespace Util
{
	inline std::vector<ConVar *> createdConvars = {};

	bool RegisterConVar( const char *name, const char *value, uint32_t flags, const char *desc, float fDefault, int iDefault, ConVarType_t type );
}
