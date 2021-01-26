#ifndef COMMON_H
#define COMMON_H

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include <ciso646>
#include <tuple>
#include <algorithm>
#include <fstream>

#define NOMINMAX
#include <windows.h>

namespace shadertest {
	
	extern std::ofstream log_file;
	extern std::wofstream wlog_file;
	
}

#endif