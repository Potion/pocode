#include "Helpers.h"
#include <cstdarg>
#include <cstdio>
#include <deque>

#ifdef __APPLE__
#include <mach/mach_time.h>
#include <sys/param.h>
#include <sys/sysctl.h>

#include <Foundation/Foundation.h>

double getTime() {
	static uint64_t start = 0.0;
    static mach_timebase_info_data_t info;
    if(info.denom == 0) {
        mach_timebase_info(&info);
		start = mach_absolute_time();
	}
    
    uint64_t duration = mach_absolute_time() - start;
    return ((duration * info.numer) / (double)info.denom) * 1.0e-9;
}

unsigned int getNumCpus() {
    int count;
    size_t size = sizeof(count);
    if(sysctlbyname("hw.ncpu", &count, &size, NULL, 0))
        return 1;
    return (unsigned int)count;
}

fs::path currentPath() {
	return fs::current_path();
}

void setCurrentPath(const fs::path &path) {
	NSString *nsstr = [NSString stringWithCString:path.c_str() encoding:NSUTF8StringEncoding];
	[[NSFileManager defaultManager] changeCurrentDirectoryPath:nsstr];
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

