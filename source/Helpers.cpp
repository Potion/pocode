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
#include "poOpenGLState.h"

#ifdef __APPLE__
#include <mach/mach_time.h>
#include <sys/param.h>
#include <sys/sysctl.h>

#ifdef POTION_IOS
	#include <UIKit/UIKit.h>
#else
	#include <Cocoa/Cocoa.h>
#endif

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
#ifdef POTION_IOS
    return poPoint(72.f, 72.f);
#else
	NSWindow *window = (NSWindow*)applicationCurrentWindow()->getWindowHandle();
	NSScreen *screen = [window screen];
	
	NSSize size = [[[screen deviceDescription] objectForKey:NSDeviceResolution] sizeValue];
	return poPoint(size.width, size.height);
#endif
}

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

bool lookUpAndSetPathNextTo(const std::string &folder_name) {
    fs::path path;
    if(pathToFolder(folder_name, &path)) {
        setCurrentPath(path);
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
	poPoint trans = obj->position;
	poPoint off = obj->offset;
	float rotation = obj->rotation;
	poPoint rot_axis = obj->rotationAxis;
	poPoint scale = obj->scale;
	
	poMatrixStack *stack = &poOpenGLState::get()->matrix;
	
	switch(obj->matrixOrder) {
		case PO_MATRIX_ORDER_TRS:
			stack->translate(trans);
			stack->rotate(rotation, rot_axis);
			stack->scale(scale);
			break;
			
		case PO_MATRIX_ORDER_RST:
			stack->rotate(rotation, rot_axis);
			stack->scale(scale);
			stack->translate(trans);
			break;
	}

	stack->translate(off);
}

//void startMasking(poShape2D *mask) {
//	poMatrixStack *stack = &poOpenGLState::get()->matrix;
//	applyObjTransform(mask);
//	
//	glEnable(GL_STENCIL_TEST);
//	
////	glClear(GL_STENCIL_BUFFER_BIT);
//	
//	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
//	glStencilFunc(GL_ALWAYS, 1, 1);
//	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//	drawPoints(GL_TRIANGLE_FAN, mask->getPoints());
//	
//	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
//	glStencilFunc(GL_EQUAL, 1, 1);
//	
//	stack->pushModelview();
//}
//
//void stopMasking() {
//	glDisable(GL_STENCIL_TEST);
//}
//
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

poPoint alignInRect(poPoint max, poRect rect, poAlignment align) {
	poPoint offset;
	switch(align) {
		case PO_ALIGN_TOP_LEFT:
			offset.set(0.f, 0.f, 0.f);
			break;
		case PO_ALIGN_TOP_CENTER:
			offset.set((max.x - 1.f)/2.f, 0.f, 0.f);
			break;
		case PO_ALIGN_TOP_RIGHT:
			offset.set(max.x - 1.f, 0.f, 0.f);
			break;
		case PO_ALIGN_CENTER_LEFT:
			offset.set(0.f, (max.y - 1.f)/2.f, 0.f);
			break;
		case PO_ALIGN_CENTER_CENTER:
			offset.set((max.x - 1.f)/2.f, (max.y - 1.f)/2.f, 0.f);
			break;
		case PO_ALIGN_CENTER_RIGHT:
			offset.set(max.x - 1.f, (max.y - 1.f)/2.f, 0.f);
			break;
		case PO_ALIGN_BOTTOM_LEFT:
			offset.set(0.f, max.y - 1.f, 0.f);
			break;
		case PO_ALIGN_BOTTOM_CENTER:
			offset.set((max.x - 1.f)/2.f, max.y - 1.f, 0.f);
			break;
		case PO_ALIGN_BOTTOM_RIGHT:
			offset.set(max.x - 1.f, max.y - 1.f, 0.f);
			break;
	}
	return offset;
}

static const std::string base64_chars = 
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

static inline bool is_base64(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];
	
	while (in_len--) {
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;
			
			for(i = 0; (i <4) ; i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}
	
	if (i)
	{
		for(j = i; j < 3; j++)
			char_array_3[j] = '\0';
		
		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;
		
		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];
		
		while((i++ < 3))
			ret += '=';
		
	}
	
	return ret;
	
}

std::string base64_decode(std::string const& encoded_string) {
	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;
	
	while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i ==4) {
			for (i = 0; i <4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);
			
			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
			
			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}
	
	if (i) {
		for (j = i; j <4; j++)
			char_array_4[j] = 0;
		
		for (j = 0; j <4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);
		
		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
		
		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}
	
	return ret;
}


int poToInt(std::string s) {
    return atoi(s.c_str());
}
