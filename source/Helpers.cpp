#include "Helpers.h"
#include <cstdarg>
#include <cstdio>
#include <deque>

#ifdef __APPLE__
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

#endif

int utf8strlen(const std::string &str) {
	int i=0, j=0;
	while (str[i]) {
		if ((str[i] & 0xC0) != 0x80)
			j++;
		i++;
	}
	return j;
}
