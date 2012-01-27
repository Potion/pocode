/*
 *	Copyright 2012 Potion Design. All rights reserved.
 *	This file is part of pocode.
 *
 *	pocode is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as 
 *	published by the Free Software Foundation, either version 3 of 
 *	the License, or (at your option) any later version.
 *
 *	pocode is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public 
 *	License along with pocode.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifdef __APPLE__
	#define POTION_APPLE
	#include "TargetConditionals.h"
	#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
		#include <OpenGLES/ES2/gl.h>
		#include <OpenGLES/ES2/glext.h>
		#define OPENGL_ES
		#define POTION_IOS
	#elif TARGET_OS_MAC
		#define POTION_MAC
		#include <OpenGL/OpenGL.h>
		#include <OpenGL/glu.h>
	#endif
#elif defined(_WIN32)
	#define POTION_WIN32
	#include "glew.h"
	#include "wglew.h"
	#include <Windows.h>
#elif defined(__linux)
// include whatever linux uses
#endif

typedef unsigned int uint;
typedef unsigned char ubyte;
typedef unsigned short ushort;
