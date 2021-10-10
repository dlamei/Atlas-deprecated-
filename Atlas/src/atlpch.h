#pragma once

#include <iostream>
#include <functional>
#include <memory>
#include <algorithm>
#include <utility>

#include <string>
#include <sstream>
#include <vector>
#include <charconv>
#include <unordered_map>
#include <unordered_set>

#include "Atlas/Core/Log.h"
#include "Atlas/Debug/Instrumentor.h"

#ifdef ATL_PLATFORM_WINDOWS
	#include <windows.h>
#endif
