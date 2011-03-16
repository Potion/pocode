#include <windows.h>
#include <varargs.h>
#include "PlatformHelpers.h"

// returns milliseconds since bootup
double getTime() {
	LARGE_INTEGER ticks_per_sec;
	LARGE_INTEGER tick;

	QueryPerformanceFrequency(&ticks_per_sec);
	QueryPerformanceCounter(&tick);
	return (tick.QuadPart / (double)ticks_per_sec.QuadPart);
}

unsigned int getNumCpus() {
	SYSTEM_INFO info;

	info.dwNumberOfProcessors = 1;

	GetSystemInfo(&info);
	if(info.dwNumberOfProcessors)
		return info.dwNumberOfProcessors;

	return 1;
}
#endif

void dprintf(const char *format, ...) {
	char buf[4096], *p = buf;
	int n;

	va_list args;
	va_start(args, format);
	n = _vsnprintf(p, sizeof buf - 3, format, args); // buf-3 is room for CR/LF/NUL
	va_end(args);

	p += (n < 0) ? sizeof buf - 3 : n;

	while ( p > buf  &&  isspace(p[-1]) )
		*--p = '\0';

	*p++ = '\r';
	*p++ = '\n';
	*p   = '\0';

	OutputDebugString(buf);
}


