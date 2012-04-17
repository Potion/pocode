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

#pragma once


#include "poRect.h"
#include "poEnums.h"

#include <vector>
#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>

#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

// in seconds
float poGetElapsedTime();
int    poGetElapsedTimeMillis();

typedef struct {
    int hours;
    int amPmHours;
    std::string amPm;
    
    int minutes;
    int seconds;
} poTime;

poTime poGetCurrentTime();

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
std::string getApplicationSupportDirectory();

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

//Convert to string to title case
std::string poToUpperCase(std::string s);
std::string poToLowerCase(std::string s);
std::string poToTitleCase(std::string s);


//Convert to string to int
int poToInt(std::string s);



