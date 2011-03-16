#include "PlatformHelpers.h"
#include <cstdarg>
#include <cstdio>

#include <mach/mach_time.h>
#include <sys/param.h>
#include <sys/sysctl.h>

double getTime() {
    static mach_timebase_info_data_t info;
    if(info.denom == 0)
        mach_timebase_info(&info);
    
    uint64_t duration = mach_absolute_time();
    return (duration * info.numer) / (double)info.denom;
}

unsigned int getNumCpus() {
    int count;
    size_t size = sizeof(count);
    if(sysctlbyname("hw.ncpu", &count, &size, NULL, 0))
        return 1;
    return (unsigned int)count;
}

void dprintf(const char *format, ...) {
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}