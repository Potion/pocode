#include "Helpers.h"
#include <cstdarg>
#include <cstdio>
#include <deque>
#include <fstream>
#include <stdarg.h>
#include <ctime>
#include <utf8.h>

#include "poObject.h"
#include "poShape2D.h"
#include "poWindow.h"
#include "poApplication.h"

#ifdef __APPLE__
#include <mach/mach_time.h>
#include <sys/param.h>
#include <sys/sysctl.h>

#include <Cocoa/Cocoa.h>
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

#if not defined(TESTING)
poPoint deviceResolution() {
	NSWindow *window = (NSWindow*)applicationCurrentWindow()->getWindowHandle();
	NSScreen *screen = [window screen];
	
	NSSize size = [[[screen deviceDescription] objectForKey:NSDeviceResolution] sizeValue];
	return poPoint(size.width, size.height);
}
#else
poPoint deviceResolution() {
	return poPoint(72,72);
}
#endif

void setCurrentPath(const fs::path &path) {
	NSString *nsstr = [NSString stringWithCString:path.c_str() encoding:NSUTF8StringEncoding];
	[[NSFileManager defaultManager] changeCurrentDirectoryPath:nsstr];
}

#endif

fs::path currentPath() {
	return fs::current_path();
}

bool pathToFolder(const std::string &folder_name, fs::path *path) {
	fs::path response = currentPath();
	while(!fs::exists(response / folder_name) && !fs::is_directory(response / folder_name) && response != "/") {
		response = response.parent_path();
	}
	
	if(response == "/") {
		*path = "";
		return false;
	}
	
	*path = response;
	return true;
}

bool lookUpAndSetPath(const std::string &folder_name) {
	fs::path path;
	if(pathToFolder(folder_name, &path)) {
		setCurrentPath(path/folder_name);
		return true;
	}
	return false;
}

std::vector<poPoint> roundedRect(float width, float height, float rad) {
	std::vector<poPoint> response;
	std::vector<poPoint> tmp;
	
	tmp = quadTo(poPoint(0,rad), poPoint(rad, 0), poPoint(0, 0), 10);
	response.insert(response.end(), tmp.begin(), tmp.end());
	
	tmp = quadTo(poPoint(width-rad,0), poPoint(width,rad), poPoint(width, 0), 10);
	response.insert(response.end(), tmp.begin(), tmp.end());
	
	tmp = quadTo(poPoint(width,height-rad), poPoint(width-rad, height), poPoint(width, height), 10);
	response.insert(response.end(), tmp.begin(), tmp.end());
	
	tmp = quadTo(poPoint(rad,height), poPoint(0, height-rad), poPoint(0, height), 10);
	response.insert(response.end(), tmp.begin(), tmp.end());
	
	return response;
}

std::vector<poPoint> quadTo(poPoint p1, poPoint p2, poPoint control, int resolution) {
	std::vector<poPoint> response;
	for(int i=0; i<resolution; i++) {
		float t = i / float(resolution-1);
		float invt = 1.f - t;
		poPoint pt = invt*invt*p1 + 2*invt*t*control + t*t*p2;
		response.push_back(pt);
	}
	return response;
}

std::vector<poPoint> cubeTo(poPoint p1, poPoint p2, poPoint c1, poPoint c2, int resolution) {
	std::vector<poPoint> response;
	for(int i=0; i<resolution; i++) {
		float t = i / float(resolution-1);
		float invt = 1.f - t;
		poPoint pt = invt*invt*invt*p1 + 3*invt*invt*t*c1 + 3*invt*t*t*c2 + t*t*t*p2;
		response.push_back(pt);
	}
	return response;
}

float curveLength(const std::vector<poPoint> &curve) {
	float len = 0;
	for(int i=0; i<curve.size()-1; i++) {
		len += (curve[i+1] - curve[i]).length();
	}
	return len;
}

std::ofstream log_file;

const char *currentTimeStr() {
	static char buffer[80];
	
	time_t now = time(NULL);
	strftime(buffer,80,"%I:%M:%S %p",localtime(&now));
	
	return buffer;
}

void applyObjTransform(poObject *obj) {
	poPoint trans = obj->position();
	poPoint off = obj->offset();
	float rotation = obj->rotation();
	poPoint rot_axis = obj->rotationAxis();
	poPoint scale = obj->scale();
	
	switch(obj->matrixOrder()) {
		case PO_MATRIX_ORDER_TRS:
			glTranslatef(trans.x, trans.y, trans.z);
			glRotatef(rotation, rot_axis.x, rot_axis.y, rot_axis.z);
			glScalef(scale.x, scale.y, scale.z);
			break;
			
		case PO_MATRIX_ORDER_RST:
			glRotatef(rotation, rot_axis.x, rot_axis.y, rot_axis.z);
			glScalef(scale.x, scale.y, scale.z);
			glTranslatef(trans.x, trans.y, trans.z);
			break;
	}
	
	glTranslatef(off.x, off.y, off.z);
}

void startMasking(poShape2D *mask) {
	glPushMatrix();
	applyObjTransform(mask);
	
	glPushAttrib(GL_STENCIL_BUFFER_BIT);
	glEnable(GL_STENCIL_TEST);
	
	glClear(GL_STENCIL_BUFFER_BIT);
	
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	drawPoints(mask->getPoints());
	
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilFunc(GL_EQUAL, 1, 1);
	
	glPopMatrix();
}

void stopMasking() {
	glPopAttrib();
}

void log(const char *format, ...) {
	static char buffer[SHRT_MAX];

	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);
	
	if(!log_file.is_open())
		log_file.open("log.text");

	std::stringstream ss;
	ss << currentTimeStr() << ": " << buffer << "\n";
	
	log_file << ss.str();
	std::cerr << ss.str();
}

size_t utf8strlen(const std::string &str) {
	return utf8::unchecked::distance(str.begin(), str.end());
}
