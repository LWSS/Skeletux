#pragma once

#include <string>
#include <sys/mman.h>
#include <zconf.h>
#include <vector>
#include <map>
#include <algorithm>
#include <chrono>
#include <locale>
#include <codecvt>
#include <sstream>
#include <link.h>
#include "../SDK/SDK.h"

namespace Util
{
	std::wstring StringToWstring(std::string str);
	long GetEpochTime();
}