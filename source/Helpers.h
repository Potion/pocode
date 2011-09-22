#pragma once

#include "poRect.h"
#include "poFont.h"
#include "poTexture.h"

#include <sstream>
#include <boost/lexical_cast.hpp>

// in seconds
double getTime();
unsigned int getNumCpus();
poPoint deviceResolution();

// path related
fs::path currentPath();
void setCurrentPath(const fs::path &path);
// search up the filesystem from pwd for folder
bool pathToFolder(const std::string &folder_name, fs::path *path);
// search up the filesystem from pwd and set to a folder if its there
bool lookUpAndSetPath(const std::string &folder_name);
bool lookUpAndSetPathNextTo(const std::string &folder_name);

// point related
std::vector<poPoint> roundedRect(float w, float h, float r);
std::vector<poPoint> quadTo(poPoint p1, poPoint p2, poPoint control, int resolution);
std::vector<poPoint> cubeTo(poPoint p1, poPoint p2, poPoint c1, poPoint c2, int resolution);
float curveLength(const std::vector<poPoint> &curve);

class poObject;
class poShape2D;

void applyObjTransform(poObject *obj);
//void startMasking(poShape2D *mask);
//void stopMasking();

void log(const char *format, ...);
size_t utf8strlen(const std::string &str);

// get useful information about images and formats
GLenum formatForBitDepth(enum ImageBitDepth bpp);
ImageBitDepth bitDepthForFormat(GLenum format);

poPoint alignInRect(poPoint max, poRect rect, poAlignment align);

/* 
 base64.cpp and base64.h
 
 Copyright (C) 2004-2008 René Nyffenegger
 
 This source code is provided 'as-is', without any express or implied
 warranty. In no event will the author be held liable for any damages
 arising from the use of this software.
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 
 1. The origin of this source code must not be misrepresented; you must not
 claim that you wrote the original source code. If you use this source code
 in a product, an acknowledgment in the product documentation would be
 appreciated but is not required.
 
 2. Altered source versions must be plainly marked as such, and must not be
 misrepresented as being the original source code.
 
 3. This notice may not be removed or altered from any source distribution.
 
 René Nyffenegger rene.nyffenegger@adp-gmbh.ch
 */

std::string base64_encode(unsigned char const* data, unsigned int len);
std::string base64_decode(std::string const& s);


//Convert anything to string (templated so put it here)
template <class T>
inline std::string poToString (const T& t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
};


//Convert to string to int
int poToInt(std::string s);



