// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <GL\glew.h>
#include <GL\wglew.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <map>
#include <list>
#include <cstdint>
#include <algorithm>

#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

namespace fs = boost::filesystem;

typedef uint32_t uint;
typedef uint8_t ubyte;