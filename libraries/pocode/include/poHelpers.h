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

namespace po {
    typedef struct {
        int hours;
        int amPmHours;
        std::string amPm;
        
        int minutes;
        int seconds;
    } Time;
    
    //Time Functions
    float getElapsedTime();
    int   getElapsedTimeMillis();

    po::Time getCurrentTime();

    
    unsigned int getNumCpus();
    Point deviceResolution();

    //Log
    void log(const char *format, ...);
    size_t utf8strlen(const std::string &str);





    //Convert anything to string (templated so put it here)
    template <class T>
    inline std::string toString (const T& t) {
        std::stringstream ss;
        ss << t;
        return ss.str();
    };

    //Convert string to title case
    std::string toUpperCase(std::string s);
    std::string toLowerCase(std::string s);
    std::string toTitleCase(std::string s);


    //Convert to string to int
    int toInt(std::string s);
}


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
namespace po {
    std::string base64_encode(unsigned char const* data, unsigned int len);
    std::string base64_decode(std::string const& s);
}