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

#include "poHelpers.h"
#include <cstdarg>
#include <cstdio>
#include <deque>
#include <fstream>
#include <sstream>
#include <stdarg.h>
#include <ctime>
#include <utf8.h>

#include "poObject.h"
#include "poShape2D.h"
#include "poWindow.h"
#include "poApplication.h"
#include "poOpenGLState.h"

#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace boost::gregorian;
using namespace boost::posix_time;

//Header Includes based on platform
#ifdef __APPLE__
    #include <mach/mach_time.h>
    #include <sys/param.h>
    #include <sys/sysctl.h>

    #if defined(POTION_MAC)
        #include <Cocoa/Cocoa.h>
    #else // IPHONE OR SIMULATOR
        #include <UIKit/UIKit.h>
    #endif
#endif

//Obj-C Classes can not exist within C++ Namespaces,
//so we use internal Mac & iOS functions here
#ifdef __APPLE__
    #if defined(POTION_MAC)
        Point getDeviceResolutionMac() {
            NSWindow *window = (NSWindow*)po::applicationCurrentWindow()->getWindowHandle();
            NSScreen *screen = [window screen];
            
            NSSize size = [[[screen deviceDescription] objectForKey:NSDeviceResolution] sizeValue];
            return Point(size.width, size.height);
        }
    #else
        Point getDeviceResolutioniOS() {
            return Point(72,72);
        }
    #endif
#endif



namespace po {
    
    // -----------------------------------------------------------------------------------
    // ================================ Time Functions ===================================
    #pragma mark - Time Functions -
    
    #ifdef __APPLE__
        float getElapsedTime() {
            static uint64_t start = 0.0;
            static mach_timebase_info_data_t info;
            if(info.denom == 0) {
                mach_timebase_info(&info);
                start = mach_absolute_time();
            }
            
            uint64_t duration = mach_absolute_time() - start;
            return ((duration * info.numer) / (double)info.denom) * 1.0e-9;
        }

        int getElapsedTimeMillis() {
            return getElapsedTime() * 1000.0f;
        }

    #elif defined(POTION_WINDOWS)
        float getElapsedTime() {
            static __int64 freq=0L, start;

            if(freq == 0) {
                // start
                QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
                QueryPerformanceCounter((LARGE_INTEGER*)&start);
            }

            // end
            __int64 end;
            QueryPerformanceCounter((LARGE_INTEGER*)&end);
            double diff = (end - start) / (double)freq;
            return diff;
        }

        int getElapsedTimeMillis() {
            return GetElapsedTime() * 1000.0f;
        }
    #endif
    
    
    //------------------------------------------------------------------------
    //Returns the current time as a Time Object
    po::Time getCurrentTime() {
        date today(day_clock::local_day());
            
        ptime now = second_clock::local_time();
            
        Time t;
        t.hours      = now.time_of_day().hours();
        t.minutes    = now.time_of_day().minutes();
        t.seconds    = now.time_of_day().seconds();
            
        //Am/PM
        t.amPmHours   = t.hours;
            
        t.amPm = t.amPmHours < 12 ? "AM" : "PM";
            
        if(t.amPmHours > 12) t.amPmHours -= 12;
        if(t.amPmHours == 0) t.amPmHours = 12;
            
        return t;
    }
    
    
    //------------------------------------------------------------------------
    //Get the current time as a string
    const char *currentTimeStr() {
        static char buffer[80];
        
        time_t now = time(NULL);
        strftime(buffer,80,"%I:%M:%S %p",localtime(&now));
        
        return buffer;
    }
    
    
    unsigned int getNumCpus() {
        return boost::thread::hardware_concurrency();
    }
    
    
    //------------------------------------------------------------------------
    //Device Resolution
    #pragma mark Device Resolution
    
    #ifdef __APPLE__
        #if defined(POTION_MAC)
            Point deviceResolution() {
                return getDeviceResolutionMac();
            }
        #else // IPHONE OR SIMULATOR
            Point deviceResolution() {
                return getDeviceResolutioniOS();
            }
        #endif
    #elif defined(POTION_WINDOWS)
        Point deviceResolution() {
            return Point(72, 72);
        }
    #endif
    
    
    //------------------------------------------------------------------------
    //Logging
    std::ofstream log_file;
    
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
    
    
    //------------------------------------------------------------------------
    size_t utf8strlen(const std::string &str) {
        using utf8::unchecked::next;
        return utf8::unchecked::distance(str.begin(), str.end());
    }
    
    
    
    
    // -----------------------------------------------------------------------------------
    // ====================== Base64 Encoding/Decoding ===================================
    #pragma mark - Time Functions -
    
    
    static const std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

    static inline bool is_base64(unsigned char c) {
        return (isalnum(c) || (c == '+') || (c == '/'));
    }
    
    
    //------------------------------------------------------------------------
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
    
    
    //------------------------------------------------------------------------
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
    
    
    //------------------------------------------------------------------------
    //String Manipulation
    #pragma mark String Manipulation
    std::string toUpperCase(std::string s) {
        for(int i=0; i<s.length(); i++) {
            s[i] = toupper(s[i]);
        }
        
        return s;
    }
    
    
    //------------------------------------------------------------------------
    std::string toLowerCase(std::string s) {
        for(int i=0; i<s.length(); i++) {
            s[i] = tolower(s[i]);
        }
        
        return s;
    }
    
    
    //------------------------------------------------------------------------
    std::string toTitleCase(std::string s) {
        for(int i=0; i<s.length(); i++) {
            if(i==0 || s[i-1] == ' ') {
                s[i] = toupper(s[i]);
            }
        }
        
        return s;
    }
    
    
    //------------------------------------------------------------------------
    int toInt(std::string s) {
        return atoi(s.c_str());
    }
} /* End po Namespace */