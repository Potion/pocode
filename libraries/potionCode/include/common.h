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
