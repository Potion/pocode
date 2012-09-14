//
//  poFileLocation.cpp
//  pocode
//
//  Created by Steve Varga on 9/6/12.
//  Copyright (c) 2012 Potion Design. All rights reserved.
//

#include "poFileLocation.h"
#include "poHelpers.h"
#include "poApplication.h"

//------------------------------------------------------------------
//Set Current Path, depends on OS
#ifdef __APPLE__
#include <mach/mach_time.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#include <Foundation/Foundation.h>

//Mac Uses NSFileManager
void setCurrentPath(const fs::path &path) {
    NSString *nsstr = [NSString stringWithCString:path.c_str() encoding:NSUTF8StringEncoding];
    [[NSFileManager defaultManager] changeCurrentDirectoryPath:nsstr];
}
#elif defined(POTION_WINDOWS)
//Windows Uses boost::filesystem to set path
void setCurrentPath(const fs::path &path) {
    fs::current_path(path);
}
#endif


//------------------------------------------------------------------
//------------------------------------------------------------------
//File Path Utils
#pragma mark FilePath Utils

namespace po {
    //------------------------------------------------------------------
    poFilePath getCurrentPath() {
        return poFilePath(fs::current_path());
    }

    
    //------------------------------------------------------------------
    bool pathToFolder(const std::string &folder_name, fs::path *path) {
        fs::path response(getCurrentPath().toString());
        
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

    
    //------------------------------------------------------------------
    bool lookUpAndSetPath(const std::string &folder_name) {
        fs::path path;
        if(pathToFolder(folder_name, &path)) {
            setCurrentPath(path/folder_name);
            return true;
        }
        return false;
    }

    
    //------------------------------------------------------------------
    bool lookUpAndSetPathNextTo(const std::string &folder_name) {
        fs::path path;
        if(pathToFolder(folder_name, &path)) {
            setCurrentPath(path);
            return true;
        }
        return false;
    }
    

    //------------------------------------------------------------------
    std::string getApplicationSupportDirectory() {
        return applicationGetSupportDirectory();
    }
}


//------------------------------------------------------------------
//poFilePath
#pragma mark poFilePath
poFilePath::poFilePath() {}

poFilePath::~poFilePath() {}

poFilePath::poFilePath(const char *path) {
    set(path);
}

poFilePath::poFilePath(const std::string &path) {
    set(path);
}

poFilePath::poFilePath(const fs::path &filePath) {
    setFromBoostPath(filePath);
}


//------------------------------------------------------------------
void poFilePath::set(std::string path) {
    this->filePath = fs::path(filePath);
}


//------------------------------------------------------------------
void poFilePath::setFromBoostPath(fs::path filePath) {
    this->filePath = filePath;
}


//------------------------------------------------------------------
bool poFilePath::isSet() const {
    return filePath.string() != "";
}


//------------------------------------------------------------------
std::string poFilePath::toString() const {
    return filePath.string();
}


//------------------------------------------------------------------
fs::path poFilePath::toBoostPath() const {
    return this->filePath;
}


//------------------------------------------------------------------
bool poFilePath::exists() const {
    return fs::exists(filePath);
}

//------------------------------------------------------------------
//Add @scale x to item, see if it exists, return new filePath
poFilePath poFilePath::getScaled(float scale) const {
    std::string stem = filePath.stem().string();
    stem += "@" + po::toString(po::toString(scale) + "x");
    std::string extension = filePath.extension().string();
    
    fs::path base(this->toString());
    poFilePath p(base.remove_filename().string() + "/" + stem + extension);
    
    if(!p.exists()) {
        p.set(this->toString());
    }
    
    return p;
}


//------------------------------------------------------------------
//------------------------------------------------------------------
//poURL
#pragma mark poURL
poURL::poURL() {}

poURL::poURL(std::string u)
: url(u)
{}

poURL::~poURL() {}

//------------------------------------------------------------------
std::string poURL::toString() const {
    return url;
}

//------------------------------------------------------------------
bool poURL::isSet() const {
    return url != "";
}