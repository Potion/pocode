#include "Helpers.h"
#include <cstdarg>
#include <cstdio>
#include <deque>

#include "poWindow.h"
#include "poApplication.h"

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

poPoint deviceResolution() {
	NSWindow *window = (NSWindow*)applicationCurrentWindow()->osDependentHandle();
	NSScreen *screen = [window screen];
	
	NSSize size = [[[screen deviceDescription] objectForKey:NSDeviceResolution] sizeValue];
	return poPoint(size.width, size.height);
}

void setCurrentPath(const fs::path &path) {
	NSString *nsstr = [NSString stringWithCString:path.c_str() encoding:NSUTF8StringEncoding];
	[[NSFileManager defaultManager] changeCurrentDirectoryPath:nsstr];
}

#endif

fs::path currentPath() {
	return fs::current_path();
}
